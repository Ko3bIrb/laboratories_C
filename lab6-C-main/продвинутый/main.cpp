#include "Comparison.h"

int main() {
    setupConsole();
    if (!compareFibonacci()) return 1;
    return showSortComparison() ? 0 : 1;
}
