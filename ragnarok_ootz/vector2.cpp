#include "vector2.h"

const Vector2 Vector2::zero = Vector2(0, 0);
const Vector2 Vector2::one = Vector2(1, 1);
const Vector2 Vector2::left = Vector2(-1, 0);
const Vector2 Vector2::upLeft = Vector2(-1, -1);
const Vector2 Vector2::up = Vector2(0, -1);
const Vector2 Vector2::upRight = Vector2(1, -1);
const Vector2 Vector2::right = Vector2(1, 0);
const Vector2 Vector2::downRight = Vector2(1, 1);
const Vector2 Vector2::down = Vector2(0, 1);
const Vector2 Vector2::downLeft = Vector2(-1, 1);

Vector2::Vector2()
    : x(0.0f)
    , y(0.0f)
{
}

Vector2::~Vector2()
{
}

Vector2& Vector2::operator+=(Vector2 const& other)
{
    x += other.x;
    y += other.y;

    return *this;
}

std::pair<int, int> Vector2::toIntPair()
{
    return std::make_pair(static_cast<int>(x), static_cast<int>(y));
}

Vector2 Vector2::Zero()
{
    return Vector2(0.0f, 0.0f);
}

Vector2 Vector2::One()
{
    return Vector2(1.0f, 1.0f);
}

Vector2 Vector2::Left()
{
    return Vector2(-1, 0);
}

Vector2 Vector2::UpLeft()
{
    return Vector2(-1, -1);
}

Vector2 Vector2::Up()
{
    return Vector2(0, -1);
}

Vector2 Vector2::UpRight()
{
    return Vector2(1, -1);
}

Vector2 Vector2::Right()
{
    return Vector2(1, 0);
}

Vector2 Vector2::DownRight()
{
    return Vector2::One();
}

Vector2 Vector2::Down()
{
    return Vector2(0, 1);
}

Vector2 Vector2::DownLeft()
{
    return Vector2(-1, 1);
}

Vector2 operator+(Vector2 const& lhs, Vector2 const& rhs)
{
    return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
}

Vector2 operator-(Vector2 const& lhs, Vector2 const& rhs)
{
    return Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
}

bool operator==(Vector2 const& lhs, Vector2 const& rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

Vector2 DotProduct(Vector2 const& lhs, Vector2 const& rhs)
{
    return Vector2(lhs.x * rhs.x, lhs.y * rhs.y);
}

std::string std::to_string(const Vector2 & vec) {
    return '(' + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ')';
}
