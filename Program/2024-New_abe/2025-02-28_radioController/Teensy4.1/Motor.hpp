#pragma once

#include <Arduino.h>

// -100~100[%] の範囲で指定

class Motor{
private:
  // 定数
  static constexpr uint8_t NUM = 4;
  static constexpr uint8_t PWM_TYPE = 2;
  static constexpr uint32_t FREQUENCY = 50000;
  const uint8_t PIN[NUM][PWM_TYPE]{
    // for Main ver.002
    {9, 8},
    {7, 6},
    {5, 4},
    {3, 2}
    
    // for Main ver.001
    // {29, 28},
    // {9, 6},
    // {5, 4},
    // {3, 2}
  };
  static constexpr uint8_t EN = 0;
  static constexpr uint8_t PH = 1;
  
  float motor         [NUM] = {0};  // プログラマ用
  float motor_prev    [NUM] = {0};  // 前ループのプログラマ用の値
  float motor_add     [NUM] = {0};  // Rawに加算(主に姿勢制御)

  // static constexpr uint8_t QUEUE_SIZE = 16;   // 出力値の移動平均のサンプル数
  static constexpr uint8_t QUEUE_SIZE = 16;   // Test 出力値の移動平均のサンプル数
  float queue[QUEUE_SIZE][NUM]{};             // 出力値のキュー
public:
  float motor_raw[NUM] = {0};  // モーターに反映するやつ
  float raw_sum = 0;

  float balance[NUM] = {0};

  void begin();

  void set(float m1, float m2, float m3, float m4);
  void add(float m1, float m2, float m3, float m4);
  void addRaw(float m1, float m2, float m3, float m4);

  void setDir      (float dir, float dir_prev, float p_gain, float d_gain);
  void setDirAdd   (float dir, float dir_prev, float p_gain, float d_gain);
  void setDirAdd   (float dir, float dir_prev, float p_gain, float d_gain, float limit);
  void setDirAddRaw(float dir, float dir_prev, float p_gain, float d_gain);

  void moveDir    (float dir, uint8_t power); // 全方位に対して同じ速度で移動
  void moveDirFast(float dir, uint8_t power); // 常にどれかのモーターの絶対値が100%の状態で動くように移動
  void moveDirAdd(float dir, uint8_t power);  // 出力値を加算
  void moveDirFastAdd(float dir, uint8_t power);


  void p();
  void avr();
  void write();
};

extern Motor motor;
