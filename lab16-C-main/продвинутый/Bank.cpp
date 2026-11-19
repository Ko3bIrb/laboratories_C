#include "Bank.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

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

// Строим счёт из общих данных. Суммы хранятся в целых копейках.
Account::Account(int id, std::string owner, Money balance) : id(id), owner(std::move(owner)), balance(balance) {
    if (id < 1 || this->owner.empty()) throw std::invalid_argument("Нужны номер и владелец");
}

int Account::number() const { return id; }

const std::string& Account::holder() const { return owner; }

Money Account::funds() const { return balance; }

// Проверяем будущий баланс до изменения счёта.
void Account::check(Money nextBalance) const {
    if (nextBalance < -limit() || nextBalance > maxMoney) {
        throw std::out_of_range("Баланс выйдет за допустимый предел");
    }
}

// Вызывается после проверки и записи операции в журнал.
void Account::setBalance(Money nextBalance) { balance = nextBalance; }

int SavingsAccount::type() const { return 1; }

const char* SavingsAccount::title() const { return "Сберегательный"; }

Money SavingsAccount::limit() const { return 0; }

// Начисляем 5% от остатка за один период.
Money SavingsAccount::interest() const { return funds() * 5 / 100; }

int CheckingAccount::type() const { return 2; }

const char* CheckingAccount::title() const { return "Расчётный"; }

Money CheckingAccount::limit() const { return 0; }

Money CheckingAccount::interest() const { return 0; }

int CreditAccount::type() const { return 3; }

const char* CreditAccount::title() const { return "Кредитный"; }

Money CreditAccount::limit() const { return 100000; }

// При долге удерживаем 12% от суммы долга за период.
Money CreditAccount::interest() const { return funds() < 0 ? funds() * 12 / 100 : 0; }

// Возвращаем счёт с данным номером или сообщаем об ошибке.
Account& Bank::account(int id) {
    for (auto& item : accounts) {
        if (item->number() == id) return *item;
    }
    throw std::out_of_range("Счёт не найден");
}

// Проверяем номер и баланс перед тем, как сохранять объект в коллекции.
void Bank::add(std::unique_ptr<Account> item) {
    for (const auto& existing : accounts) {
        if (existing->number() == item->number()) throw std::invalid_argument("Номер уже занят");
    }
    item->check(item->funds());
    accounts.push_back(std::move(item));
}

// Общая операция для пополнения, снятия и начисления процентов.
void Bank::change(Account& item, Money amount, const char* operation) {
    Money next = item.funds() + amount;
    item.check(next);
    journal.push_back(std::string(operation) + " #" + std::to_string(item.number())
                      + " " + std::to_string(amount) + " коп.");
    item.setBalance(next);
}

// Новый счёт открывается с нулевым балансом.
void Bank::open(int type, int id, const std::string& owner) {
    add(makeAccount(type, id, owner, 0));
}

// Для пополнения передаём положительный знак, для снятия — отрицательный.
void Bank::deposit(int id, Money amount) {
    if (amount == 0 || amount > maxMoney || amount < -maxMoney) {
        throw std::out_of_range("Укажите допустимую ненулевую сумму");
    }
    change(account(id), amount, amount > 0 ? "Пополнение" : "Снятие");
}

// Сначала проверяем оба остатка. После записи в журнал меняем их вместе.
void Bank::transfer(int from, int to, Money amount) {
    if (from == to || amount <= 0 || amount > maxMoney) throw std::invalid_argument("Неверный перевод");
    auto& sender = account(from);
    auto& receiver = account(to);
    // До изменения счетов проверяем, что перевод допустим для обеих сторон.
    sender.check(sender.funds() - amount);
    receiver.check(receiver.funds() + amount);
    journal.push_back("Перевод #" + std::to_string(from) + " -> #" + std::to_string(to)
                      + ": " + std::to_string(amount) + " коп.");
    sender.setBalance(sender.funds() - amount);
    receiver.setBalance(receiver.funds() + amount);
}

// Ставка зависит от реального типа счёта.
void Bank::applyInterest(int id) {
    auto& item = account(id);
    change(item, item.interest(), "Проценты");
}

// Фабрика использует тот же код типа в меню и при загрузке.
std::unique_ptr<Account> makeAccount(int type, int id, std::string owner, Money balance) {
    switch (type) {
        case 1: return std::make_unique<SavingsAccount>(id, std::move(owner), balance);
        case 2: return std::make_unique<CheckingAccount>(id, std::move(owner), balance);
        case 3: return std::make_unique<CreditAccount>(id, std::move(owner), balance);
        default: throw std::invalid_argument("Неизвестный тип счёта");
    }
}

// Записываем версию, счета и журнал в текстовом формате.
    void Bank::save(const std::filesystem::path& path) const {
        std::ofstream file(path);
        if (!file) throw std::runtime_error("Не удалось открыть файл для записи");
        file << "BANK1 " << accounts.size() << ' ' << journal.size() << '\n';
        for (const auto& item : accounts) {
            file << item->type() << ' ' << item->number() << ' ' << item->funds()
                 << ' ' << std::quoted(item->holder()) << '\n';
        }
        for (const auto& entry : journal) file << std::quoted(entry) << '\n';
        file.close();
        if (!file) throw std::runtime_error("Ошибка сохранения");
    }

    // Читаем во временный банк. Подменяем текущие данные только после всех проверок.
    void Bank::load(const std::filesystem::path& path) {
        std::ifstream file(path);
        std::string format;
        std::size_t count, entries;
        if (!(file >> format >> count >> entries) || format != "BANK1"
            || count > 100000 || entries > 100000) throw std::runtime_error("Неверный файл банка");
        Bank next;
        for (std::size_t i = 0; i < count; ++i) {
            int type, id;
            Money balance;
            std::string owner;
            if (!(file >> type >> id >> balance >> std::quoted(owner))) throw std::runtime_error("Неверная запись счёта");
            next.add(makeAccount(type, id, owner, balance));
        }
        for (std::size_t i = 0; i < entries; ++i) {
            std::string entry;
            if (!(file >> std::quoted(entry))) throw std::runtime_error("Неверная запись журнала");
            next.journal.push_back(std::move(entry));
        }
        file >> std::ws;
        if (!file.eof() || file.bad()) throw std::runtime_error("Лишние данные в файле банка");
        // К этому моменту файл целиком проверен. При ошибке исходный банк остаётся прежним.
        *this = std::move(next);
    }

// Возвращаем строку целиком, чтобы в имени владельца могли быть пробелы.
std::string readLine(const char* prompt) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) throw std::runtime_error("Ввод завершён");
    if (line.empty()) throw std::invalid_argument("Строка не должна быть пустой");
    return line;
}

// Принимаем ровно одно целое число в указанном диапазоне.
template<class Number>
Number readNumber(const char* prompt, Number low, Number high) {
    std::istringstream input(readLine(prompt));
    Number value;
    if (!(input >> value)) throw std::invalid_argument("Ожидалось целое число");
    input >> std::ws;
    if (!input.eof()) throw std::invalid_argument("Ожидалось одно целое число");
    if (value < low || value > high) throw std::out_of_range("Число вне диапазона");
    return value;
}

// Номера счетов в меню всегда положительные.
int readId(const char* prompt = "Номер счёта: ") { return readNumber(prompt, 1, 1000000); }
// Команды меню вызывают методы банка. После ошибки можно продолжить работу.

void runBankMenu() {
    Bank bank;
    while (std::cin) {
        std::cout << "\n1 Открыть | 2 Счета | 3 Пополнить | 4 Снять | 5 Перевести\n"
                  << "6 Проценты | 7 Журнал | 8 Сохранить | 9 Загрузить | 0 Выход\n";
        try {
            int choice = readNumber("Команда: ", 0, 9);
            if (choice == 0) break;
            switch (choice) {
                case 1: {
                    int type = readNumber("Тип (1 сберегательный, 2 расчётный, 3 кредитный): ", 1, 3);
                    int id = readId();
                    bank.open(type, id, readLine("Владелец: "));
                    break;
                }
                case 2:
                    if (bank.list().empty()) std::cout << "Счетов пока нет.\n";
                    for (const auto& item : bank.list()) {
                        std::cout << '#' << item->number() << ' ' << item->title() << ", " << item->holder()
                                  << ", баланс " << item->funds() << " коп.\n";
                    }
                    break;
                case 3:
                case 4: {
                    int id = readId();
                    auto amount = readNumber("Сумма в копейках: ", 1LL, maxMoney);
                    bank.deposit(id, choice == 3 ? amount : -amount);
                    break;
                }
                case 5: {
                    int from = readId("Счёт отправителя: ");
                    int to = readId("Счёт получателя: ");
                    bank.transfer(from, to, readNumber("Сумма в копейках: ", 1LL, maxMoney));
                    break;
                }
                case 6: bank.applyInterest(readId()); break;
                case 7:
                    if (bank.history().empty()) std::cout << "Транзакций пока нет.\n";
                    for (const auto& entry : bank.history()) std::cout << entry << '\n';
                    break;
                case 8:
                    bank.save(std::filesystem::u8path(readLine("Файл: ")));
                    std::cout << "Сохранено.\n";
                    break;
                case 9:
                    bank.load(std::filesystem::u8path(readLine("Файл: ")));
                    std::cout << "Загружено.\n";
                    break;
            }
        } catch (const std::exception& error) {
            if (std::cin) std::cout << "Ошибка: " << error.what() << '\n';
        }
    }
}
