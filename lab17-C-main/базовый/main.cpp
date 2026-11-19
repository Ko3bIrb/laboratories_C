#include "EmployeeDirectory.h"

int main() {
    setupConsole();
    EmployeeDirectory directory;
    directory.add(makeEmployee(1, 11, "Анна", "Разработка", 9, 180000));
    directory.add(makeEmployee(2, 12, "Иван", "Разработка", 4, 120000));
    directory.add(makeEmployee(3, 13, "Ольга", "Стажировка", 0, 40000));

    // Через один интерфейс выводим разные должности и их действия.
    std::cout << "Сотрудники по зарплате:\n";
    directory.sort(1);
    directory.showAll();
    std::cout << "\nОтдел «Разработка»:\n";
    directory.showMatches([](const Employee& employee) { return employee.unit() == "Разработка"; });
}
