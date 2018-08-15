#pragma once

#include <type_traits>
#include <cassert>
#include <utility>
#include <string>

struct Vector2
{
    Vector2();
    template<typename T1, typename T2>
    Vector2(const T1& x, const T2& y);

    ~Vector2();

    Vector2& operator+=(const Vector2& other);

    std::pair<int, int> toIntPair();

    template<typename T1, typename T2>
    static Vector2 From(const std::pair<T1, T2>& other);

    static Vector2 Zero();
    static Vector2 One();
    static Vector2 Left();
    static Vector2 UpLeft();
    static Vector2 Up();
    static Vector2 UpRight();
    static Vector2 Right();
    static Vector2 DownRight();
    static Vector2 Down();
    static Vector2 DownLeft();

    static const Vector2 zero;
    static const Vector2 one;
    static const Vector2 left;
    static const Vector2 upLeft;
    static const Vector2 up;
    static const Vector2 upRight;
    static const Vector2 right;
    static const Vector2 downRight;
    static const Vector2 down;
    static const Vector2 downLeft;

    float x;
    float y;
};

Vector2 operator+(Vector2 const& lhs, Vector2 const& rhs);
Vector2 operator-(Vector2 const& lhs, Vector2 const& rhs);
bool operator==(Vector2 const& lhs, Vector2 const& rhs);

template<typename T>
Vector2 operator*(const Vector2& lhs, const T& rhs);
template<typename T>
Vector2 operator*(const T& lhs, const Vector2& rhs);

Vector2 DotProduct(Vector2 const& lhs, Vector2 const& rhs);

namespace std {
std::string to_string(const Vector2& vec);
}

template<typename T1, typename T2>
Vector2::Vector2(const T1& x, const T2& y)
{
    assert(std::is_arithmetic<T1>::value &&
           std::is_arithmetic<T2>::value && "Vector2::Vector2<T1, T2>()");

    this->x = static_cast<float>(x);
    this->y = static_cast<float>(y);
}

template<typename T1, typename T2>
Vector2 Vector2::From(const std::pair<T1, T2>& other)
{
    assert(std::is_arithmetic<T1>::value &&
           std::is_arithmetic<T2>::value);

    return Vector2(other.first, other.second);
}

template<typename T>
Vector2 operator*(const Vector2& lhs, const T& rhs)
{
    assert(std::is_arithmetic<T>::value && "operator*<Vector2, T>()");

    return Vector2(lhs.x * rhs, lhs.y * rhs);
}

template<typename T>
Vector2 operator*(const T& lhs, const Vector2& rhs)
{
    return rhs * lhs;
}
