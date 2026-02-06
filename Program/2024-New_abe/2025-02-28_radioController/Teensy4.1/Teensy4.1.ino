// new atacker ritsumori
// Teensy4.1
// 2024-02-23

#include <Arduino.h>

#include <sstream>
#include <cmath>
#include<vector>
#include<string>

#include <Wire.h>

#include "ball.hpp"
#include "dir.hpp"
#include "Display.hpp"
#include "Motor.hpp"
#include "Sub.hpp"
#include "UI.hpp"

Ball ball;
Dir dir;
Display display;
Motor motor;
Sub sub;
UI ui;


using namespace std;


bool is_display_on = true;

float offset = 1.0;



void setup() {
  Serial.begin(115200);
  Serial.println("2024-12-24 TOINIOT2 kansai");

  ball.begin();
  display.begin();
  // dir.begin();
  motor.begin();
  sub.begin();
  ui.begin();

  // calibration
  uint8_t system=0, gyro=0, accel=0, mag=0;
  system = 3;
  gyro = 3;
  mag = 3;
  while(system<3 || gyro<3 || mag<3){
    dir.calibration(&system, &gyro, &accel, &mag);
    digitalWrite(LED_BUILTIN, HIGH);

    display.printd(32,16,"system: "+to_string(system));
    display.printd(32,24,"gyro  : "+to_string(gyro));
    display.printd(32,32,"accel : "+to_string(accel));
    display.printd(32,40,"mag   : "+to_string(mag));

    display.printd(120, 8, "skip", ALIGN::RIGHT);
    ui.read();
    if(ui.buttonUp(1)) break;

    display.draw();
  }

  // 攻め方向が設定されるまで待機
  display.printd(8,56,"set dir");
  display.draw();
  while(!ui.buttonUp(0)){ ui.read(); }
  display.draw();

  dir.setDefault();

  Serial.println("start");
}



std::vector<std::string> splitBySpace(const std::string& str) {
    std::vector<std::string> result;
    std::istringstream iss(str);
    std::string word;
    while (iss >> word) {
        result.push_back(word);
    }
    return result;
}

int safeStoi(const std::string& str, int defaultValue = 0) {
    return std::stoi(str);
}


void loop() {
  ball.read();
  // dir.read();
  sub.read();
  ui.read();
  


  auto strs = splitBySpace(sub.str);
  int x=0, y=0;
  int rotate = 0;
  int boost=0, kick=0, reset=0;
  if(strs.size() > 6){
    for(size_t i=0;i<strs.size();i++){ 
      if(i == 0) x = safeStoi(strs[i], 0);
      if(i == 1) y = safeStoi(strs[i], 0);
      // if(i == 2) rotate = safeStoi(strs[i], 0);  // 正面から見る場合
      if(i == 3) rotate = safeStoi(strs[i], 0);     // 横から見る場合
      if(i == 4) boost = safeStoi(strs[i], 0);
      if(i == 5) kick = safeStoi(strs[i], 0);
      if(i == 6) reset = safeStoi(strs[i], 0);
    }
  }

  double move_dir = 0;
  if(x == 0){
    x = 1;
  }
  move_dir = degrees(atan2((double)y,(double)x)) + 90.0 - dir.dir;
  double power = sqrt(x*x+y*y)*120.0 / 1414.0;
  if(power > 100.0) power = 100.0;
  // Serial.printf("dir:%lf power:%lf\n", move_dir, power);

  if(kick){
    sub.kick();
  }
  if(power > 0.1){
    if(boost){
      motor.moveDirFast(move_dir, 100);
    }else{
      motor.moveDir(move_dir, 45);
    }

  }else{
    motor.moveDir(0,0);
  }

  double ofs = rotate / 45.0;
  motor.setDirAdd(dir.dir - ofs, dir.dir_prev, dir.p_gain, dir.d_gain);

  // display.addValiables("x:"+to_string(x), nullptr);
  // display.addValiables("y:"+to_string(y), nullptr);


  sub.send(to_string(ball.dir));



  // ディスプレイ
  if(ui.is_toggle){
    // Serial.println("display");

    if(ui.buttonUp(0)) display.next();

    display.addValiables("m1:"+to_string(motor.balance[0] * 10), &motor.balance[0]);
    display.addValiables("m2:"+to_string(motor.balance[1] * 10), &motor.balance[1]);
    display.addValiables("m3:"+to_string(motor.balance[2] * 10), &motor.balance[2]);
    display.addValiables("m4:"+to_string(motor.balance[3] * 10), &motor.balance[3]);

    display.debug();
    display.draw();
    is_display_on = true;

    motor.set(0,0,0,0);

    motor.avr();
    motor.write();
    delay(4);

    return;
  }
  // ディスプレイを消灯
  if(is_display_on){
    display.draw();
    is_display_on = false;
  }

  delay(3);
  






  motor.avr();
  motor.write();
}