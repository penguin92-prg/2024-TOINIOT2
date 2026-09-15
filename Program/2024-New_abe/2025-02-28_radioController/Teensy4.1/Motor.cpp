#include "Motor.hpp"

// ---------------- 基本関数 ----------------

void Motor::begin(){
  for(int i=0;i<NUM;i++){
    pinMode(PIN[i][EN], OUTPUT);
    pinMode(PIN[i][PH], OUTPUT);
    balance[i] = 10.0;
    analogWriteFrequency(PIN[i][EN], FREQUENCY);
  }

  balance[1] = 8.0;
  
  Serial.println("motor setup");
  return;
}



void Motor::set(float m1, float m2, float m3, float m4){
  motor[0] = m1;
  motor[1] = m2;
  motor[2] = m3;
  motor[3] = m4;

  return;
}


void Motor::add(float m1, float m2, float m3, float m4){
  motor[0] += m1;
  motor[1] += m2;
  motor[2] += m3;
  motor[3] += m4;

  return;
}


void Motor::addRaw(float m1, float m2, float m3, float m4){
  motor_add[0] = m1;
  motor_add[1] = m2;
  motor_add[2] = m3;
  motor_add[3] = m4;

  return;
}


// ---------------- 姿勢制御 ----------------


void Motor::setDir(float dir, float dir_prev, float p_gain, float d_gain){
  float power = dir * p_gain + (dir - dir_prev) * d_gain;
  set(power, power, power, power);  
  
  return;
}


void Motor::setDirAdd(float dir, float dir_prev, float p_gain, float d_gain){
  float power = dir * p_gain + (dir - dir_prev) * d_gain;

  if(power > 100)  power = 100;
  if(power < -100) power = -100;
  
  add(power, power, power, power);  
  
  return;
}


void Motor::setDirAdd(float dir, float dir_prev, float p_gain, float d_gain, float limit){
  float power = dir * p_gain + (dir - dir_prev) * d_gain;

  if(power > limit)  power = limit;
  if(power < -limit) power = -limit;
  
  add(power, power, power, power);  
  
  return;
}


void Motor::setDirAddRaw(float dir, float dir_prev, float p_gain, float d_gain){
  float power = dir * p_gain + (dir - dir_prev) * d_gain;
  addRaw(power, power, power, power);  
  
  return;
}


// ---------------- 移動 ----------------


void Motor::moveDir(float dir, uint8_t power){
  for(int i=0;i<NUM;i++){
    float t = (-dir-45-i*90)*3.14/180.0;
    motor[i] = sin(t)*power;
  }

  return;
}


void Motor::moveDirFast(float dir, uint8_t power){
  moveDir(dir, power);

  float max = 0;
  for(int i=0;i<NUM;i++){
    if(abs(motor[i]) > max){
      max = abs(motor[i]);
    }
  }

  for(int i=0;i<NUM;i++){
    motor[i] = motor[i] * power / max;
  }

  return;
}


// ---------------- 平均化・反映 ----------------


void Motor::avr() {
  static int   queue_index = 0;       // 出力値のキューのインデックス
  static float queue_sum[NUM] = {0};  // 各モーターのキュー内の合計値を記録

  for (int i = 0; i < NUM; i++) {
    // キューの合計値から古い値を引き、新しい値を足す
    queue_sum[i] -= queue[queue_index][i];
    queue[queue_index][i] = motor[i];
    queue_sum[i] += motor[i];

    // 平均値を計算
    motor_raw[i] = queue_sum[i] / (float)QUEUE_SIZE;
  }

  // インデックスを更新
  queue_index = (queue_index + 1) % QUEUE_SIZE;

  return;
}


void Motor::write(){
  raw_sum = 0;
  for(int i=0;i<NUM;i++){
    motor_raw[i] += motor_add[i];
    motor_add[i] = 0;

    motor_raw[i] = motor_raw[i]<-100.0 ? -100.0 : motor_raw[i];
    motor_raw[i] = 100.0<motor_raw[i]  ?  100.0 : motor_raw[i];

    raw_sum += abs(motor_raw[i]);


    if(i != 3)  digitalWrite( PIN[i][PH], motor_raw[i]<0 );
    else        digitalWrite( PIN[i][PH], motor_raw[i]>0 );
    analogWrite ( PIN[i][EN], (uint8_t)abs(motor_raw[i]*balance[i]*255.0/1000.0) );
  }

  return;
}
