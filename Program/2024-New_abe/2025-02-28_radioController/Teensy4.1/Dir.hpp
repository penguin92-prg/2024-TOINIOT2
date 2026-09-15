#pragma once

#include <Arduino.h>
#define byte uint8_t

#include <Adafruit_BNO055.h>
#include <Wire.h>

#include "Vec2.hpp"

class Dir{
private:
  Adafruit_BNO055 bno;

  float default_dir;

  float queue[20]{};
  int   id = 0;

public:
  float dir;
  float dir_prev;

  float x, y, z;
  float accel_avr;

  float p_gain = 0.45, d_gain = 0.64;

  Dir() : bno(55, 0x28, &Wire2), default_dir(0), dir(0), dir_prev(0) {};

  void begin();
  void calibration(uint8_t* system, uint8_t* gyro, uint8_t* accel, uint8_t* mag);
  void setDefault();
  void read();
};

extern Dir dir;