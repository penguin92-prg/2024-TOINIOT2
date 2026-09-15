#pragma once

#include <string>

#include <Arduino.h>

class Sub{
  static constexpr uint16_t INTERVAL = 6000;
  uint32_t kicked_timer = 0;
public:
  std::string str = "";
  void begin();
  void read();
  void send(std::string str);
  void kick();
};

extern Sub sub;