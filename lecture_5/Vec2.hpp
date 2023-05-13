//
// Created by Nikita Fomichev on 13/5/23.
//

#ifndef COMP4300_VEC2_HPP
#define COMP4300_VEC2_HPP

class Vec2
{
public:
    float x = 0, y = 0;
    Vec2(float xin, float yin);
    bool operator == (const Vec2& rhs) const;
    Vec2 operator + (const Vec2& rhs) const;
    Vec2 operator * (float val) const;
    void operator += (const Vec2& rhs);
    friend std::ostream& operator << (std::ostream& os, const Vec2& vec);
};

#endif //COMP4300_VEC2_HPP
