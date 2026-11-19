#include "ExternalSort.h"
#include <iostream>
#include <stdexcept>
namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    setupConsole();
    try {
        if (argc == 3) {
            auto runs = externalSort(fs::u8path(argv[1]), fs::u8path(argv[2]));
            std::cout << "Начальных серий: " << runs << ". Результат записан.\n";
        } else if (argc == 1) {
            compare();
        } else {
            throw std::invalid_argument("Укажите входной и выходной файлы");
        }
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
