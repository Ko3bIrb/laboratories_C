#pragma once
#include <algorithm>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

class Employee {
    int id;
    std::string name, department;
    int experience, salary;

public:
    Employee(int id, std::string name, std::string department, int experience, int salary);

    virtual ~Employee() = default;
    virtual int type() const = 0;
    virtual const char* title() const = 0;
    virtual const char* duty() const = 0;
    int number() const;
    const std::string& fullName() const;
    const std::string& unit() const;
    int years() const;
    int pay() const;

    void print() const;

    void write(std::ostream& file) const;
};

class Manager : public Employee {
public:
    using Employee::Employee;
    int type() const override;
    const char* title() const override;
    const char* duty() const override;
};

class Developer : public Employee {
public:
    using Employee::Employee;
    int type() const override;
    const char* title() const override;
    const char* duty() const override;
};

class Intern : public Employee {
public:
    using Employee::Employee;
    int type() const override;
    const char* title() const override;
    const char* duty() const override;
};

std::unique_ptr<Employee> makeEmployee(int type, int id, const std::string& name, const std::string& department, int years, int salary);

class EmployeeDirectory {
    std::vector<std::unique_ptr<Employee>> employees;

    void requireEmployees() const;

public:
    void add(std::unique_ptr<Employee> employee);

    void remove(int id);

    const Employee& at(int position) const;

    void sort(int criterion);

    // Поиск печатает все записи, удовлетворяющие переданному условию.
    template<class Match>
    void showMatches(Match match) const {
        requireEmployees();
        bool found = false;
        for (const auto& employee : employees) {
            if (match(*employee)) {
                employee->print();
                found = true;
            }
        }
        if (!found) std::cout << "Совпадений нет.\n";
    }

    void showAll() const;

    void save(const std::filesystem::path& path) const;
    void load(const std::filesystem::path& path);
};

void setupConsole();
