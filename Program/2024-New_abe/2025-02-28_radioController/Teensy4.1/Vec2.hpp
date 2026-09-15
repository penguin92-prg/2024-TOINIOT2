#pragma once

#include <Arduino.h>

class Vec2{
public:
  float x, y;
  Vec2() : x(0.0f), y(0.0f){}
  Vec2(float vec_x, float vec_y) : x(vec_x), y(vec_y){}

  Vec2& operator+=(const Vec2& v);
  Vec2& operator-=(const Vec2& v);
  Vec2& operator*=(const float f);
  Vec2& operator/=(const float f);
  Vec2& operator=(const Vec2& v);

  void set(float vec_x, float vec_y);
  void clear(); 
  float len();
};

Vec2 operator+(const Vec2& v1, const Vec2& v2);
Vec2 operator-(const Vec2& v1, const Vec2& v2);
Vec2 operator*(const Vec2& v1, const float f);
Vec2 operator*(const float f, const Vec2& v);

float dotProduct(Vec2 a, Vec2 b);