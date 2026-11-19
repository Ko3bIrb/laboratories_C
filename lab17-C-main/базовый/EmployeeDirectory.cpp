#include "EmployeeDirectory.h"


#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

// Задаём кодировку консоли Windows для русских сообщений.
void setupConsole() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

// Здесь же проверяем данные, которые могут прийти как с клавиатуры, так и из файла.
Employee::Employee(int id, std::string name, std::string department, int experience, int salary)
    : id(id), name(std::move(name)), department(std::move(department)),
          experience(experience), salary(salary) {
    if (this->name.find_first_not_of(" \t\r\n") == std::string::npos
        || this->department.find_first_not_of(" \t\r\n") == std::string::npos) {
        throw std::invalid_argument("Имя и отдел должны быть заполнены");
    }
    if (id < 1 || id > 1000000 || experience < 0 || experience > 80
        || salary < 0 || salary > 1000000000) throw std::out_of_range("Поля сотрудника вне диапазона");
}

int Employee::number() const { return id; }

const std::string& Employee::fullName() const { return name; }

const std::string& Employee::unit() const { return department; }

int Employee::years() const { return experience; }

int Employee::pay() const { return salary; }

// Общие поля печатаются здесь, должность и обязанности выбирает производный класс.
void Employee::print() const {
    std::cout << '#' << id << ' ' << name << " | " << title() << " | " << department
              << " | " << experience << " лет | " << salary << " руб. | " << duty() << '\n';
}

// Числовой код типа помогает восстановить реальный класс при загрузке.
void Employee::write(std::ostream& file) const {
    file << type() << ' ' << id << ' ' << experience << ' ' << salary << ' '
         << std::quoted(name) << ' ' << std::quoted(department) << '\n';
}

int Manager::type() const { return 1; }

const char* Manager::title() const { return "Менеджер"; }

const char* Manager::duty() const { return "распределяет задачи"; }

int Developer::type() const { return 2; }

const char* Developer::title() const { return "Разработчик"; }

const char* Developer::duty() const { return "пишет код"; }

int Intern::type() const { return 3; }

const char* Intern::title() const { return "Стажёр"; }

const char* Intern::duty() const { return "учится у наставника"; }

// Общая проверка для операций, которым нужен хотя бы один объект.
void EmployeeDirectory::requireEmployees() const {
    if (employees.empty()) throw std::logic_error("Список сотрудников пуст");
}

// Проверяем, свободен ли ID, и только потом принимаем объект.
void EmployeeDirectory::add(std::unique_ptr<Employee> employee) {
    if (employees.size() >= 10000) throw std::out_of_range("В справочнике уже 10000 сотрудников");
    auto sameNumber = [&](const auto& current) { return current->number() == employee->number(); };
    if (std::any_of(employees.begin(), employees.end(), sameNumber)) {
        throw std::invalid_argument("Сотрудник с таким ID уже существует");
    }
    employees.push_back(std::move(employee));
}

// Удаляем по ID, потому что порядковый номер может меняться после сортировки.
void EmployeeDirectory::remove(int id) {
    requireEmployees();
    auto match = std::find_if(employees.begin(), employees.end(),
                              [id](const auto& employee) { return employee->number() == id; });
    if (match == employees.end()) throw std::out_of_range("Сотрудник с таким ID не найден");
    employees.erase(match);
}

// Номер строки начинается с 1. Проверяем обе границы до обращения к вектору.
const Employee& EmployeeDirectory::at(int position) const {
    requireEmployees();
    if (position < 1 || static_cast<std::size_t>(position) > employees.size()) {
        throw std::out_of_range("В списке нет строки с таким номером");
    }
    return *employees[static_cast<std::size_t>(position - 1)];
}

// Равные значения сохраняют прежний порядок.
void EmployeeDirectory::sort(int criterion) {
    requireEmployees();
    std::stable_sort(employees.begin(), employees.end(), [criterion](const auto& a, const auto& b) {
        if (criterion == 1) return a->pay() < b->pay();
        if (criterion == 2) return a->years() < b->years();
        return a->fullName() < b->fullName();
    });
}

// Тот же обход с условием «подходит каждый» показывает всю коллекцию.
void EmployeeDirectory::showAll() const {
    showMatches([](const Employee&) { return true; });
}

// Фабрика создаёт сотрудника нужной должности и возвращает указатель на Employee.
std::unique_ptr<Employee> makeEmployee(int type, int id, const std::string& name,
                                             const std::string& department, int years, int salary) {
    switch (type) {
        // Тот же числовой код используется и в меню, и при чтении файла.
        case 1: return std::make_unique<Manager>(id, name, department, years, salary);
        case 2: return std::make_unique<Developer>(id, name, department, years, salary);
        case 3: return std::make_unique<Intern>(id, name, department, years, salary);
        default: throw std::invalid_argument("Неизвестная должность");
    }
}
