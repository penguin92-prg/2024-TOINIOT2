#include <Adafruit_NeoPixel.h>

constexpr uint8_t NEOPIXEL_PIN = 10;

Adafruit_NeoPixel strip(60, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  Serial.println("TOINIOT2 NeoPixel Test 2024-08-28");

  strip.begin();
  strip.show();

  Serial.println(strip.numPixels());
}

uint32_t dir = 0;

void loop() {
  dir%=360;

  // RGB 計算
  short h = dir%360;
  short s = 128;
  short v = 256;

  strip.setBrightness(32);
  for(int i=0;i<16;i++){
    double r=0;
    double g=0;
    double b=0;

    double max = v;
    double min = max-(s*max/255.0);

    // double hue = h;
    double hue = h+i*22.5;
    if(hue>=360.0) hue -= 360.0;

    if(hue<60.0){
      r = max;
      g = (hue/60.0)*(max-min)+min;
      b = min;
    }else if(60.0<=hue&&hue<120.0){
      r = ((120.0-hue)/60.0)*(max-min)+min;
      g = max;
      b = min;
    }else if(120.0<=hue&&hue<180.0){
      r = min;
      g = max;
      b = ((hue-120.0)/60.0)*(max-min)+min;
    }else if(180.0<=hue&&hue<240.0){
      r = min;
      g = ((240.0-hue)/60.0)*(max-min)+min;
      b = max;
    }else if(240.0<=hue&&hue<300.0){
      r = ((hue-240.0)/60.0)*(max-min)+min;
      g = min;
      b = max;
    }else if(300.0<=hue){
      r = max;
      g = min;
      b = ((360.0-hue)/60.0)*(max-min)+min;
    }

    strip.setPixelColor(i,(uint8_t)r,(uint8_t)g,(uint8_t)b);
    // Serial.printf("%lf ", hue);
  }

  Serial.println();
  // Serial.printf("dir:%d hue:%d sat:%d val:%d \n", dir, h, s, v);

  dir+=8;
  strip.show();
  delay(20);
}
