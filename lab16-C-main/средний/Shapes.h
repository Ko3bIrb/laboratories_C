#pragma once
#include <cmath>
#include <memory>
#include <stdexcept>

class Shape {
    static inline int alive = 0;

protected:
    static double positive(double value);

public:
    Shape();
    Shape(const Shape&);
    virtual ~Shape();
    static int count();

    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual void scale(double factor) = 0;
    virtual std::unique_ptr<Shape> clone() const = 0;
    virtual const char* name() const = 0;
};

enum class ShapeType { circle, rectangle, triangle };
std::unique_ptr<Shape> makeShape(ShapeType type, double a, double b = 0, double c = 0);

void setupConsole();
