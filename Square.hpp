#ifndef SQUARE_HPP
#define SQUARE_HPP

#include <iostream>
#include <vector>
#include "Point.hpp"

template <class T>
struct Square {
    T side;
    T area;
    Point<T> leftUpperPt;
    Point<T> center;
    std::vector<Point<T>> points;

    Square() = default;

    Square(Point<T> _leftUpperPt, T _side)
        : leftUpperPt(_leftUpperPt), side(_side)
    {
        points.resize(4);
        area = side * side;
        points[0] = leftUpperPt;
        points[1] = leftUpperPt;
        points[1].x += side;
        points[2] = points[1];
        points[2].y -= side;
        points[3] = points[2];
        points[3].x -= side;
        center.x = (points[0].x + points[1].x) / 2;
        center.y = (points[0].y + points[2].y) / 2;
    }
};

template <class U>
std::istream& operator>>(std::istream& in, Square<U>& square)
{
    // std::cout << "Введите левую верхнюю вершину и сторону квадрата: ";
    in >> square.leftUpperPt >> square.side;
    square = Square<U>(square.leftUpperPt, square.side);
    return in;
}

template <class U>
std::ostream& operator<<(std::ostream& out, const Square<U>& square)
{
    out << '\n';
    out << "Длина стороны: " << square.side << '\n';
    out << "Площадь: " << square.area << '\n';
    out << "Центр: " << square.center << '\n';
    out << "Вершины: ";
    for (int i = 0; i < 4; ++i) {
        out << square.points[i] << " ";
    }
    out << '\n';
    return out;
}

#endif