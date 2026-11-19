#include "Shapes.h"


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

// Все размеры и коэффициенты в примере положительные и конечные.
double Shape::positive(double value) {
    if (!std::isfinite(value) || value <= 0) throw std::invalid_argument("Нужно положительное число");
    return value;
}

// Считаем и обычное создание, и копирование через clone().
Shape::Shape() { ++alive; }

Shape::Shape(const Shape&) { ++alive; }

Shape::~Shape() { --alive; }

int Shape::count() { return alive; }

constexpr double pi = 3.141592653589793;

class Circle : public Shape {
    double radius;
public:
    explicit Circle(double radius) : radius(positive(radius)) {}
    double area() const override { return pi * radius * radius; }
    double perimeter() const override { return 2 * pi * radius; }
    void scale(double factor) override { radius *= positive(factor); }
    std::unique_ptr<Shape> clone() const override { return std::make_unique<Circle>(*this); }
    const char* name() const override { return "Круг"; }
};

class RectangleShape : public Shape {
    double width, height;
public:
    RectangleShape(double width, double height) : width(positive(width)), height(positive(height)) {}
    double area() const override { return width * height; }
    double perimeter() const override { return 2 * (width + height); }
    // При масштабировании меняются обе стороны.
    void scale(double factor) override {
        factor = positive(factor);
        width *= factor;
        height *= factor;
    }
    std::unique_ptr<Shape> clone() const override { return std::make_unique<RectangleShape>(*this); }
    const char* name() const override { return "Прямоугольник"; }
};

class Triangle : public Shape {
    double a, b, c;
public:
    Triangle(double a, double b, double c) : a(positive(a)), b(positive(b)), c(positive(c)) {
        if (a + b <= c || a + c <= b || b + c <= a) throw std::invalid_argument("Такого треугольника нет");
    }
    // Используем формулу Герона по трём сторонам.
    double area() const override {
        double half = perimeter() / 2;
        return std::sqrt(half * (half - a) * (half - b) * (half - c));
    }
    double perimeter() const override { return a + b + c; }
    void scale(double factor) override {
        factor = positive(factor);
        a *= factor;
        b *= factor;
        c *= factor;
    }
    std::unique_ptr<Shape> clone() const override { return std::make_unique<Triangle>(*this); }
    const char* name() const override { return "Треугольник"; }
};

// Фабрика выбирает конкретный класс и возвращает общий указатель Shape.
std::unique_ptr<Shape> makeShape(ShapeType type, double a, double b, double c) {
    switch (type) {
        case ShapeType::circle: return std::make_unique<Circle>(a);
        case ShapeType::rectangle: return std::make_unique<RectangleShape>(a, b);
        case ShapeType::triangle: return std::make_unique<Triangle>(a, b, c);
    }
    throw std::invalid_argument("Неизвестный вид фигуры");
}
