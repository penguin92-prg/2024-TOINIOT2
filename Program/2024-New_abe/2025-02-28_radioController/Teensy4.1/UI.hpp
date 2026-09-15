#pragma once

#include <Arduino.h>

class UI{
private:
  static constexpr uint8_t BUTTON_NUM = 4;
  const     uint8_t BUTTON_PIN[BUTTON_NUM] = { 36,30,31,32 };


  bool button[BUTTON_NUM] = {false};
  bool prev_button[BUTTON_NUM] = {false};


  static constexpr uint8_t TOGGLE_PIN = 37;
  bool is_toggle_prev = false;
  

  static constexpr uint8_t BZ_PIN = 33;
  float  bz = 0;


  uint32_t damaged_begin = 0; // 1.10  故障復帰用のタイマーの開始時間
  
  // for Main ver.001
  // static constexpr uint8_t  BUTTON_NUM = 3;
  // const uint8_t             BUTTON_PIN[BUTTON_NUM] = { 37,31,30 };  
  // static constexpr uint8_t TOGGLE_PIN = 36;
public:
  bool is_toggle = false;

  int32_t damaged_timer = 0; // 1.10  故障復帰用タイマー

  void begin();
  void read();
  bool buttonUp(uint8_t id);
  void buzzer(float frequency);
};

extern UI ui;