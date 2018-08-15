#pragma once

#include <functional>
#include <Windows.h>
#include <string>
#include <vector>

#include "vector2.h"
#include "direction.h"
#include "converter.h"
#include "game_object_manager.h"

struct Color
{
    static const COLORREF kBlack = RGB(0, 0, 0);
    static const COLORREF kGray = RGB(127, 127, 127);
    static const COLORREF kWhite = RGB(255, 255, 255);
    static const COLORREF kRed = RGB(255, 0, 0);
    static const COLORREF kGreen = RGB(0, 255, 0);
    static const COLORREF kBlue = RGB(0, 0, 255);
    static const COLORREF kCyan = RGB(0, 255, 255);
    static const COLORREF kMagenta = RGB(255, 0, 255);
    static const COLORREF kYellow = RGB(255, 255, 0);
};

struct Math
{
    static float DegreesToRadians(const float degrees)
    {
        return degrees * PI / 180.0f;
    }

    static float RadiansToDegrees(const float radians)
    {
        return radians * 180.0f / PI;
    }

    // coordinate system : Direct 2D
    static Direction GetDirectionFromTo(Vector2 const& from, Vector2 const& to)
    {
        Vector2 math_diff = Converter::Direct2DToMath2D(to - from);
        return GetDirectionFromATan2(atan2f(math_diff.y, math_diff.x));
    }

    static Vector2 GetUnitVector2FromTo(Vector2 const& from, Vector2 const& to)
    {
        switch (GetDirectionFromTo(from, to))
        {
        case Direction::kLeft:
            return Vector2::left;
        case Direction::kUpLeft:
            return Vector2::upLeft;
        case Direction::kUp:
            return Vector2::up;
        case Direction::kUpRight:
            return Vector2::upRight;
        case Direction::kRight:
            return Vector2::right;
        case Direction::kDownRight:
            return Vector2::downRight;
        case Direction::kDown:
            return Vector2::down;
        case Direction::kDownLeft:
            return Vector2::downLeft;
        default:
            return Vector2::zero;
        }
    }

    static Vector2 DirectionToUnitVector2(const Direction direction)
    {
        switch (direction)
        {
        case Direction::kDown:
            return Vector2::down;
        case Direction::kDownLeft:
            return Vector2::downLeft;
        case Direction::kDownRight:
            return Vector2::downRight;
        case Direction::kLeft:
            return Vector2::left;
        case Direction::kRight:
            return Vector2::right;
        case Direction::kUp:
            return Vector2::up;
        case Direction::kUpLeft:
            return Vector2::upLeft;
        case Direction::kUpRight:
            return Vector2::upRight;
        default:
            return Vector2::zero;
        }
    }

    static const float PI;

private:
    static Direction GetDirectionFromATan2(float const& angle)
    {
        const float PI_OVER_8 = PI / 8.0f;
        const float PI_3_OVER_8 = PI * 3.0f / 8.0f;
        const float PI_5_OVER_8 = PI * 5.0f / 8.0f;
        const float PI_7_OVER_8 = PI * 7.0f / 8.0f;

        if (angle >= PI_7_OVER_8 &&
            angle < -PI_7_OVER_8)
            return Direction::kLeft;
        else if (angle >= PI_5_OVER_8 &&
                 angle < PI_7_OVER_8)
            return Direction::kUpLeft;
        else if (angle >= PI_3_OVER_8 &&
                 angle < PI_5_OVER_8)
            return Direction::kUp;
        else if (angle >= PI_OVER_8 &&
                 angle < PI_3_OVER_8)
            return Direction::kUpRight;
        else if (angle >= -PI_OVER_8 &&
                 angle < PI_OVER_8)
            return Direction::kRight;
        else if (angle >= -PI_3_OVER_8 &&
                 angle < -PI_OVER_8)
            return Direction::kDownRight;
        else if (angle >= -PI_5_OVER_8 &&
                 angle < -PI_3_OVER_8)
            return Direction::kDown;
        else if (angle >= -PI_7_OVER_8 &&
                 angle < -PI_5_OVER_8)
            return Direction::kDownLeft;
        else
            return Direction::kLeft;
    }
};

template
    < typename T1
    , typename T2
    , typename T3
    , typename T4
>
void DrawRectangleCenter(HDC const& hdc, T1 const& center_x, T2 const& center_y, T3 const& width, T4 const& height)
{
    T1 half_width = static_cast<T1>(width / 2);
    T2 half_height = static_cast<T2>(height / 2);

    Rectangle(hdc,
              static_cast<int>(center_x - half_width), static_cast<int>(center_y - half_height),
              static_cast<int>(center_x + half_width), static_cast<int>(center_y + half_height));
}

template
    < typename T1
    , typename T2
    , typename T3
    , typename T4
>
void DrawRectangle(HDC const& hdc, T1 const& x, T2 const& y, T3 const& width, T4 const& height)
{
    Rectangle(hdc,
              static_cast<int>(x), 
              static_cast<int>(y),
              static_cast<int>(x + width), 
              static_cast<int>(y + height));
}

struct Utility
{
    static std::vector<int> GetNumberVectorFromNumber(const int number)
    {
        std::string str = std::to_string(number);
        std::vector<int> ret;
        for (auto c : str)
            ret.emplace_back(c - '0');
        return ret;
    }

    static bool IntToBool(const int number)
    {
        return number ? true : false;
    }

    static int BoolToInt(const bool var) {
        return var ? 1 : 0;
    }
};

struct Font
{
    Font(HDC const& hdc, int const& size = 1)
        : hdc(hdc)
        , size(size)
    {}

    void operator()(std::function<void()> const& functor)
    {
        HFONT font = CreateFont(size, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, TEXT("Malgun"));
        HFONT oldfont = (HFONT)SelectObject(hdc, font);

        functor();

        SelectObject(hdc, oldfont);
        DeleteObject(font);
    }

    HDC hdc;
    int size;
};

struct Pen
{
    Pen(HDC const& hdc, int const& width = 1, COLORREF const& color = Color::kBlack)
        : hdc(hdc)
        , width(width)
        , color(color)
    {}

    void operator()(std::function<void()> const& functor)
    {
        HBRUSH brush = CreateSolidBrush(color);
        HBRUSH oldbrush = (HBRUSH)SelectObject(hdc, brush);
        HPEN pen = CreatePen(PS_SOLID, width, Color::kWhite);
        HPEN oldpen = (HPEN)SelectObject(hdc, pen);

        functor();

        SelectObject(hdc, oldbrush);
        SelectObject(hdc, oldpen);
        DeleteObject(brush);
        DeleteObject(pen);
    }

    HDC hdc;
    int width;
    COLORREF color;
};

struct Paint
{
    // left top
    static void Text(const HDC hdc, const Vector2& pos, const std::string& text)
    {
        auto factor = GameObjectManager::GetFactorOfPosition();
        TextOut(hdc, static_cast<int>(pos.x + factor.x), static_cast<int>(pos.y + factor.y), text.c_str(), text.size());
    }
};