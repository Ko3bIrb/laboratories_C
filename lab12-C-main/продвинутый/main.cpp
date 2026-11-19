#include "ShortestPaths.h"
#include <iostream>

int main() {
    setupConsole();
    showDirectedExample();
    std::cout << '\n';
    return comparePaths() ? 0 : 1;
}
