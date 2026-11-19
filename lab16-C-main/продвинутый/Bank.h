#pragma once
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

void runBankMenu();

using Money = long long;
constexpr Money maxMoney = 1'000'000'000'000LL;

class Account {
    int id;
    std::string owner;
    Money balance;

public:
    Account(int id, std::string owner, Money balance);
    virtual ~Account() = default;
    int number() const;
    const std::string& holder() const;
    Money funds() const;
    virtual int type() const = 0;
    virtual const char* title() const = 0;
    virtual Money limit() const = 0;
    virtual Money interest() const = 0;

    void check(Money nextBalance) const;

    void setBalance(Money nextBalance);
};

class SavingsAccount : public Account {
public:
    using Account::Account;
    int type() const override;
    const char* title() const override;
    Money limit() const override;
    Money interest() const override;
};

class CheckingAccount : public Account {
public:
    using Account::Account;
    int type() const override;
    const char* title() const override;
    Money limit() const override;
    Money interest() const override;
};

class CreditAccount : public Account {
public:
    using Account::Account;
    int type() const override;
    const char* title() const override;
    Money limit() const override;
    Money interest() const override;
};

std::unique_ptr<Account> makeAccount(int type, int id, std::string owner, Money balance);

class Bank {
    std::vector<std::unique_ptr<Account>> accounts;
    std::vector<std::string> journal;

    Account& account(int id);

    void add(std::unique_ptr<Account> item);

    void change(Account& item, Money amount, const char* operation);

public:
    void open(int type, int id, const std::string& owner);

    void deposit(int id, Money amount);

    void transfer(int from, int to, Money amount);

    void applyInterest(int id);

    // Меню показывает коллекции через ссылки только для чтения.
    const auto& list() const { return accounts; }
    const auto& history() const { return journal; }

    void save(const std::filesystem::path& path) const;
    void load(const std::filesystem::path& path);
};

void setupConsole();
