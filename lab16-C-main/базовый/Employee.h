#pragma once
#include <string>

class Employee {
    std::string name;
    int id, experience;
public:
    Employee(std::string name, int id, int experience);
    virtual ~Employee() = default;
    virtual void work() const = 0;
    void print() const;
};
class Manager : public Employee {
public:
    using Employee::Employee;
    void manage() const;
    void work() const override;
};
class Developer : public Employee {
public:
    using Employee::Employee;
    void code() const;
    void work() const override;
};
class Intern : public Employee {
public:
    using Employee::Employee;
    void learn() const;
    void work() const override;
};

void setupConsole();
