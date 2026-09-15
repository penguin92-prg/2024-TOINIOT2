#include "UI.hpp"

void UI::begin(){
  for(auto p:BUTTON_PIN) pinMode(p, INPUT);
  pinMode(TOGGLE_PIN, INPUT_PULLUP);
  pinMode(BZ_PIN, OUTPUT);

  Serial.println("ui setup");
  
  return;
}

void UI::read(){
  for(int i=0;i<BUTTON_NUM;i++){
    prev_button[i] = button[i];
    button[i] = digitalRead(BUTTON_PIN[i]);
  }


  is_toggle_prev = is_toggle;
  is_toggle = digitalRead(TOGGLE_PIN);

  if(is_toggle_prev == false && is_toggle == true){
    damaged_begin = millis();
  }
  damaged_timer = 30000 - ( millis() - damaged_begin );

  return;
}

bool UI::buttonUp(uint8_t id){
  id = id<0            ? 0            : id;
  id = id>BUTTON_NUM-1 ? BUTTON_NUM-1 : id;

  return (!button[id]) && prev_button[id];
}

void UI::buzzer(float frequency){
  bz = frequency;
  tone(BZ_PIN, bz);

  return;
}