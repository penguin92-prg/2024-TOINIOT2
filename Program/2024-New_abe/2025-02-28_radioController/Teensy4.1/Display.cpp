#include <string>
#include "Display.hpp"

using namespace std;

void Display::begin(){
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)){
    Serial.println("Display err!");
    while(true);
  }
  display.setTextSize(1);
  display.setTextColor(WHITE);

  return;
}

void Display::clear(){
  display.clearDisplay();
  display.display();

  return;
}

void Display::draw(){
  display.display();
  display.clearDisplay();

  variables.clear();
  valiables_addr.clear();
    
  return;
}

void Display::next(){
  mode = (mode+1)%MODE_NUM;
  return;
}



void Display::drawAngleLine(uint8_t cx, uint8_t cy, float angle, uint8_t r){
  display.drawLine(cx, cy, cx+cos(radians(angle))*r, cy+sin(radians(angle))*r, WHITE);
  return;
}

void Display::printd(uint8_t x, uint8_t y, std::string str, ALIGN align_x, ALIGN align_y){
  switch(align_x){
    case ALIGN::LEFT :
      break;
    case ALIGN::CENTER :
      x -= str.length()*8/2;
      break;
    case ALIGN::RIGHT :
      x -= str.length()*8;
      break;
    default:
      break;
  }
  switch(align_y){
    case ALIGN::TOP :
      break;
    case ALIGN::MIDDLE :
      y -= 8/2;
      break;
    case ALIGN::BOTTOM :
      y -=8;
      break;
    default:
      break;
  }
  display.setCursor(x, y);
  display.print(str.c_str());

  return;
}



void Display::addValiables(std::string name, float* addr){
  variables.emplace_back(name);
  valiables_addr.emplace_back(addr);
  return;
}



void Display::debug(){
  debug(mode);
  return;
}

void Display::debug(uint8_t mode){
  switch(mode){
    case MODE::BLE :
      this->Ble();
      break;
    case MODE::KICKER :
      this->Kicker();
      break;
    case MODE::VARIABLES :
      this->Valiables();
      break;
    default:
      for(size_t i=0;i<variables.size();i++){
        printd(8,24+i*8,variables[i]);
      }
      printd(8,8,"internal error!");
      break;
  }

  return;
}



void Display::Ble(){
  printd(8,8,"BLE");
  printd(8, 24, sub.str);
  /*
  printd(8, 24, "ATK:"+BLE_atk);
  printd(8, 40, "DEF:"+BLE_def);
  */

  return;
}


void Display::Kicker(){
  printd(8,8,"Kicker");

  int width = 24;
  int height = 48;
  display.drawRect(8, 8, width, height, WHITE);
  display.drawRect(96, 8, width, height, WHITE);

  // display.fillRect(8,  8+(255-sub.brightness)*height/255, width, (sub.brightness)*height/255, WHITE);
  // display.fillRect(96, 8+(255-sub.ball02k)*height/255, width, (sub.ball02k)*height/255, WHITE);

  // printd(64,56,"hold :"+to_string(sub.is_hold),ALIGN::CENTER);
  // printd(64,48,"ready:"+to_string(sub.ready),  ALIGN::CENTER);

  printd(120,16,"test kick",ALIGN::RIGHT);
  if(ui.buttonUp(1)) sub.kick();
  printd(120,32, "reset dir",ALIGN::RIGHT);
  if(ui.buttonUp(2)) dir.setDefault();
  
  return;
}

void Display::Valiables(){
  printd(8,8,"Valiables");

  // 表示
  for(size_t i=0;i<variables.size();i++){
    printd(16,16+8*i,variables[i]);
  }

  
  // 変数のセレクタ
  if(ui.buttonUp(3)){
    selector++;
    selector %= variables.size();
  }
  printd(8,16+selector*8,">");

  printd(120,8,"+",ALIGN::RIGHT);
  printd(120,32,"-",ALIGN::RIGHT);
  printd(120,56,"o",ALIGN::RIGHT);
  printd(128,56,"select",ALIGN::RIGHT);

  // 加減算
  static float order = 0.1;
  if(ui.buttonUp(1))      *valiables_addr[selector] += order;
  else if(ui.buttonUp(2)) *valiables_addr[selector] -= order;
  
  return;
}