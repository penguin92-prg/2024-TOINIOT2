#include "Sub.hpp"

void Sub::begin(){
  Serial7.begin(115200);
  return;
}


void Sub::read(){
  if(Serial7.available()){
    str = "";
    char c = ' ';
    while(c!='_'){
      if(Serial7.available()){
        c = (char)Serial7.read();
        str += c;
      }

    }

  }

  return;
}


void Sub::send(std::string str){
  str += "_";
  Serial7.print(str.c_str());

  return;
}


void Sub::kick(){
  bool ready = millis()-kicked_timer > INTERVAL;

  if(ready){
    Serial7.print('k');
    kicked_timer = millis();
  }

  return;
}