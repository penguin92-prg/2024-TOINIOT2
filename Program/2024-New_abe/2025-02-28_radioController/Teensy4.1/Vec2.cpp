#include <cmath>

#include "Vec2.hpp"

Vec2 operator+(const Vec2& v1, const Vec2& v2){
  Vec2 v;
  v.x = v1.x + v2.x;
  v.y = v1.y - v2.y;
  return v;
}

Vec2 operator-(const Vec2& v1, Vec2& v2){
  Vec2 v;
  v.x = v1.x - v2.x;
  v.y = v1.y - v2.y;
  return v;
};

Vec2 operator*(const Vec2& v, const float f){
  Vec2 r;
  r.x = v.x * f;
  r.y = v.y * f;
  return r;
}

Vec2 operator*(const float f, const Vec2& v){
  Vec2 r;
  r.x = f * v.x;
  r.y = f * v.y;
  return r;
}

Vec2& Vec2::operator+=(const Vec2& v){
  x += v.x;
  y += v.y;
  return *this;
}

Vec2& Vec2::operator-=(const Vec2& v){
  x -= v.x;
  y -= v.y;
  return *this;
}

Vec2& Vec2::operator*=(const float f){
  x *= f;
  y *= f;
  return *this;
}

Vec2& Vec2::operator/=(const float f){
  x /= f;
  y /= f;
  return *this;
}

Vec2& Vec2::operator=(const Vec2& v){
  x = v.x;
  y = v.y;
  return *this;
}


void Vec2::set(float vec_x, float vec_y){
  x = vec_x;
  y = vec_y;
  return;
}

float Vec2::len(){
  return std::sqrt(x*x + y*y);
}

void Vec2::clear(){
  x = 0.0f;
  y = 0.0f;
  return;
}



float dotProduct(Vec2 a, Vec2 b){
  return a.x * b.x + a.y * b.y;
}