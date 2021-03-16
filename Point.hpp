#ifndef POINT_HPP
#define POINT_HPP

#include <iostream>

template <class T>
struct Point {
    T x, y;
};

template <class T>
std::istream& operator>>(std::istream& in, Point<T>& point)
{
    in >> point.x >> point.y;
    return in;
}

template <class T>
std::ostream& operator<<(std::ostream& out, const Point<T>& point)
{
    out << "(" << std::fixed << point.x << ", " << point.y << ")";
    return out;
}

#endif
