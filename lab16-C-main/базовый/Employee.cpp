#include "Employee.h"
#include <iostream>
#include <utility>

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

// Общие данные сотрудника принадлежат базовому классу.
Employee::Employee(std::string name, int id, int experience)
    : name(std::move(name)), id(id), experience(experience) {}

// Общая часть вывода одинакова; действие зависит от реальной должности.
void Employee::print() const {
    std::cout << '#' << id << ' ' << name << ", стаж: " << experience << " лет — ";
    work();
}

void Manager::manage() const { std::cout << "распределяет задачи\n"; }
void Manager::work() const { manage(); }

void Developer::code() const { std::cout << "пишет код\n"; }
void Developer::work() const { code(); }

void Intern::learn() const { std::cout << "учится у наставника\n"; }
void Intern::work() const { learn(); }
