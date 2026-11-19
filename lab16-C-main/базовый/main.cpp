#include "Employee.h"
#include <memory>
#include <vector>

int main() {
    setupConsole();
    std::vector<std::unique_ptr<Employee>> team;
    team.push_back(std::make_unique<Manager>("Анна", 1, 8));
    team.push_back(std::make_unique<Developer>("Иван", 2, 4));
    team.push_back(std::make_unique<Intern>("Ольга", 3, 0));
    for (const auto& employee : team) employee->print();
}
