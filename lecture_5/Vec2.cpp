//
// Created by Nikita Fomichev on 13/5/23.
//
#include <iostream>
#include "Vec2.hpp"

Vec2::Vec2(float xin, float yin)
        : x(xin), y(yin) {}

Vec2 Vec2::operator + (const Vec2& rhs) const
{
    return {x + rhs.x, y + rhs.y};
}

Vec2 Vec2::operator * (float rhs) const
{
    return {x * rhs, y * rhs};
}

bool Vec2::operator == (const Vec2& rhs) const
{
    return (x == rhs.x && y == rhs.y);
}

void Vec2::operator += (const Vec2& rhs)
{
    x += rhs.x;
    y += rhs.y;
}

std::ostream& operator << (std::ostream& os, const Vec2& vec)
{
    os << vec.x << " " << vec.y;
    return os;
}

int main()
{
    Vec2 v1 = {3, 4};
    Vec2 v2 = {1, 1};
    Vec2 v3 = {3, 3};
    v1 += v2;
}




