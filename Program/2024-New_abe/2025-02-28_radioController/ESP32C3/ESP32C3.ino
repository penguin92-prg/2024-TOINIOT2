#include <Adafruit_NeoPixel.h>

// central(attacker)

#include <string>
#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>


#define KICKER_PIN D3
#define NEOPIXEL_NUM 16

Adafruit_NeoPixel strip(NEOPIXEL_NUM, D10, NEO_GRB + NEO_KHZ800);


// BLE
#define CENTRAL_NAME "TOINIOT2_CONTROLLER"
#define SERVICE_UUID "6a10c527-bac9-485e-bdb8-462f3b72bd59"
#define NOTIFY_UUID "07edec9a-716c-4379-b18b-2bb8eb1a2bea"
#define CIPO_UUID "d4b0e22e-2597-48e3-886e-ea984f3f7db0"
#define COPI_UUID "2234bf79-5ff8-4b47-a139-b9a20801eaf4"

bool connected = true;
bool prev_connected = false;
std::string CIPO_str      = "";
std::string CIPO_str_prev = "";
bool is_changed = false;
BLEServer *central;
BLECharacteristic *NOTIFY = nullptr;
BLECharacteristic *CIPO = nullptr;
BLECharacteristic *COPI = nullptr;


class CentralCallbacks : public BLEServerCallbacks{
  void onConnect(BLEServer *_central){
    connected = true;
  }
  void onDisconnected(BLEServer *_central){
    connected = false;
  }
};

class CharacteristicCallbacks : public BLECharacteristicCallbacks{
  void onWrite(BLECharacteristic *_characteristic){
    String _CIPO = _characteristic -> getValue();
    if(_CIPO.length()>0){
      CIPO_str_prev = CIPO_str;
      CIPO_str = _CIPO.c_str();

      if(CIPO_str != CIPO_str_prev){
        is_changed = true;
      }

    }
  }
};



void setup() {
  Serial.begin(115200);
  Serial.println("TOINIOT2 Radio Controller");

  Serial1.setPins(D7,D6);
  Serial1.begin(115200);

  pinMode(KICKER_PIN, OUTPUT);



  // BLE
  BLEDevice::init(CENTRAL_NAME);

  central = BLEDevice::createServer();
  central -> setCallbacks(new CentralCallbacks());

  BLEService *service = central -> createService(SERVICE_UUID);

  NOTIFY = service -> createCharacteristic(NOTIFY_UUID, BLECharacteristic::PROPERTY_NOTIFY);
  NOTIFY -> addDescriptor(new BLE2902());

  CIPO = service -> createCharacteristic(CIPO_UUID, BLECharacteristic::PROPERTY_WRITE);
  CIPO -> setCallbacks(new CharacteristicCallbacks());

  COPI = service -> createCharacteristic(COPI_UUID, BLECharacteristic::PROPERTY_READ);
  COPI -> addDescriptor(new BLE2902());

  service -> start();

  central -> getAdvertising() -> start();


  // neopixel
  strip.begin();
  strip.setBrightness(192);
}




void loop() {
  // kicker
  static uint32_t kicker_timer = 0;
  static double ball_dir = 0;

  // 切り捨てる
  if(Serial1.available()){

    while(Serial1.available() > 12){
      char c = (char)Serial1.read();
      if(c == 'k'){
        if(millis()-kicker_timer > 6000){
          kicker_timer = millis();
        }
      }

    }
    while(Serial1.available() && Serial1.read() != '_'){}

    // 読み出し
    std::string str = "";
    for(int i=0;i<10;i++){
      if(Serial1.available()){
        char c = (char)Serial1.read();
        str += c;
        Serial.print(c);
      }
    }
    ball_dir = std::stod(str);

  }
  Serial.println();

  if(millis()-kicker_timer < 60){
    digitalWrite(KICKER_PIN, HIGH);
  }else{
    digitalWrite(KICKER_PIN, LOW);
  }



  // BLE
  if(connected){
    if(is_changed){
      Serial.print(CIPO_str.c_str());
      // for(int i=0;i<CIPO_str.size();i++){
      //   // if(i==CIPO_str.size()-1){
      //   // }else if(i==CIPO_str.size()-2){
      //   //   Serial.printf("%d", CIPO_str[i]);
      //   // }else{
      //   //   Serial.printf("%d ", CIPO_str[i]);
      //   // }
      //   int val = CIPO_str[i];
      //   if(CIPO_str > 127)  val = 127-val;
      //   Serial.printf("%d ", val);
      // }
      Serial1.print(CIPO_str.c_str());
      Serial1.print("_");
      is_changed = false;
    }
    // COPI -> setValue(COPI_str.c_str());
    // COPI -> notify();
  }

  if(!connected && prev_connected){
    delay(500);
    central -> startAdvertising();
    // Serial.println("\nbegan advertising\n");
    prev_connected = connected;
  }

  if(connected && !prev_connected) prev_connected = connected;


  // neopixel
  static int count = 0;
  for(int pin = 0; pin < NEOPIXEL_NUM; pin++){
    // if(count % 2 == 0){
    //   strip.setPixelColor(pin, 0, 0, 0);
    // }else{
    // }
    strip.setPixelColor(pin, 255, 64, 0);
    strip.show();
  }
  count++;
}