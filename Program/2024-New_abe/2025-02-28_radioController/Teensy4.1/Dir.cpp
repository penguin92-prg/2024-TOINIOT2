#include "Dir.hpp"

void Dir::begin(){
  if(!bno.begin()){
    Serial.println("bno error!");
    while(true);
  }

  Serial.println("dir setup");
  return;
}

void Dir::calibration(uint8_t* system, uint8_t* gyro, uint8_t* accel, uint8_t* mag){
  bno.getCalibration(system, gyro, accel, mag);
  return;
}

void Dir::setDefault(){
  sensors_event_t d{};
  bno.getEvent(&d, Adafruit_BNO055::VECTOR_EULER);
  default_dir = d.orientation.x;

  return;
}

void Dir::read(){
  dir_prev = dir;
  sensors_event_t dir_data{};
  bno.getEvent(&dir_data, Adafruit_BNO055::VECTOR_EULER);

  // 角度を更新 [-180~180]
  dir = dir_data.orientation.x-default_dir;
  if(dir<-180) dir += 360;
  if(dir>180)  dir -= 360;


  // 加速度
  sensors_event_t accel_data{};
  bno.getEvent(&accel_data, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  x = accel_data.acceleration.x + 0.3;
  y = accel_data.acceleration.y + 0.3;
  z = accel_data.acceleration.z - 9.5;
  float accel = sqrt(x*x + y*y);

  queue[id] = accel;
  id = (id+1) % 20;
  
  accel_avr = 0;
  for(auto q:queue) accel_avr += q;
  accel_avr /= 20.0f;

  
  return;
}