#include "Shapes.h"
#include <iostream>

// Печатаем площадь и периметр через виртуальные методы.
void print(const Shape& shape) {
    std::cout << shape.name() << ": площадь " << shape.area()
              << ", периметр " << shape.perimeter() << '\n';
}

int main() {
    setupConsole();
    {
        auto circle = makeShape(ShapeType::circle, 2);
        auto rectangle = makeShape(ShapeType::rectangle, 2, 5);
        auto triangle = makeShape(ShapeType::triangle, 3, 4, 5);
        auto copy = triangle->clone();
        // Масштабируем клон через указатель на Shape. Исходный треугольник не меняется.
        copy->scale(2);
        print(*circle);
        print(*rectangle);
        print(*triangle);
        std::cout << "Копия после scale(2) — ";
        print(*copy);
        std::cout << "Живых объектов: " << Shape::count() << '\n';
    }
    std::cout << "После выхода из блока: " << Shape::count() << '\n';
}
