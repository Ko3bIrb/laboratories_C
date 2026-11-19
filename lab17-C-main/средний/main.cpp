#include "EmployeeDirectory.h"

int main() {
    setupConsole();
    EmployeeDirectory directory;
    directory.add(makeEmployee(1, 11, "Анна \"А.\"", "Разработка", 9, 180000));
    directory.add(makeEmployee(2, 12, "Иван", "Разработка", 4, 120000));
    const std::filesystem::path file = "employees_demo.txt";
    try {
        directory.save(file);
        std::cout << "Сохранено: employees_demo.txt\n";
        EmployeeDirectory restored;
        restored.load(file);
        std::cout << "Восстановлены должности и поля из файла:\n";
        restored.showAll();

        // Проверяем обработку ошибки после успешной загрузки.
        restored.at(99).print();
    } catch (const std::out_of_range& error) {
        std::cout << "Ошибка доступа: " << error.what() << '\n';
    } catch (const std::runtime_error& error) {
        std::cerr << "Ошибка файла: " << error.what() << '\n';
        return 1;
    }
}
