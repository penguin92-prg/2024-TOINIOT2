constexpr uint8_t LINE_NUM = 30;
constexpr uint8_t INNER_NUM = 26;
const     uint8_t LINE_PIN[LINE_NUM] = {
  // 7,8,9,10,11, 12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52
  32,34,38,37,36, 33,39,4,12,14, 15,16,17,22,29, 24,25,26,27,42, 45,44,43,41,48, 47,46,5,23,40
};

constexpr uint8_t ANALOG_NUM = 16;
const     uint8_t ANALOG_PIN[ANALOG_NUM] = {
  // A11, A10, A2, A3, A1, A9, A8, A6, A0, A7, A5, A4
  A6, A4, A2, A15, A1,   A3, A5, A9, A11, A13,   A12, A10,    A8, A0, A7, A14
};

constexpr uint8_t THRESHOLD_PIN1 = 2; // angel
constexpr uint8_t THRESHOLD_PIN2 = 3; // outside

bool line[LINE_NUM];
int analog[ANALOG_NUM];

uint32_t max = 0;
uint32_t min = 1023;
uint32_t avr = 0;
uint32_t count = 0;

uint8_t threshold1 = 170;
uint8_t threshold2 = 200;

void setup(){
  Serial.begin(115200);
  Serial.println("TOINIOT2 Line 2025-01-05");

  // Serial1.begin(115200);

  for(auto p:LINE_PIN) pinMode(p, INPUT_PULLUP);
  // for(auto p:ANALOG_PIN) pinMode(p, INPUT_PULLUP);
  
  pinMode(THRESHOLD_PIN1, OUTPUT);
  pinMode(THRESHOLD_PIN2, OUTPUT);

  auto begin_ms = millis();
  // while(millis()-begin_ms < 300){
  //   for(int i=0;i<ANALOG_NUM;i++){
  //     int s = analogRead(ANALOG_PIN[i]);
  //     if(s > max){
  //       max = s;
  //     }else if(s<min){
  //       min = s;
  //     }
  //     else{
  //       avr += s;
  //       count++;
  //     }
  //   }
  //   delay(100);
  // }

  // avr = avr / count;
  // threshold1 = (avr*4+max)/5/4;

}

// U1
// 常に1 U5 U14 U24 U38-40

// 先頭3ビット
// 30/5 = 6

// 0b0001 1111  0

// 0b001x xxxx  1
// 0b010x xxxx  2
// 0b011x xxxx  3
// 0b100x xxxx  4
// 0b101x xxxx  5
// 0b110x xxxx  6

// 0b0000 0000  7

char send_str[8]{};
constexpr uint8_t DATA_SIZE = 5;

uint8_t *selector = &threshold1;

void loop(){
  // if(Serial1.available()){
  //   char str[10];
  //   int i = 0;
  //   while(Serial1.available()){
  //     str[i] = Serial1.read();
  //     count++;
  //   }
  //   threshold = atoi(str);
  // }
  // Serial.print(threshold*4);
  // Serial.print("\t");


  // if(Serial1.available()){
  //   char c = Serial1.read();
  //   if(c == 'c'){
  //     if(selector == &threshold1){
  //       selector = &threshold2;
  //     }else{
  //       selector = &threshold1;
  //     }
  //   }
  //   if(c == 'u') *selector += 2;
  //   if(c == 'd') *selector -= 2;
  //   // threshold -= 5;
  // }
  analogWrite(THRESHOLD_PIN1, threshold1);
  analogWrite(THRESHOLD_PIN2, threshold2);

  // 白線を取得
  for(int i=0;i<LINE_NUM;i++){
    if(i == 14 || i == 18 || i == 20 || i == 28) continue;
    line[i] = digitalRead(LINE_PIN[i]);
  }

  // 送信する文字列の準備
  for(int i=0;i<7;i++){
    send_str[i] = i << DATA_SIZE;
  }
  
  // 圧縮
  for(int i=0;i<LINE_NUM;i++){
    send_str[i/DATA_SIZE + 1] |= line[i] << (DATA_SIZE - i%DATA_SIZE - 1);
  }


  send_str[0] = 0b00011111;
  send_str[7] = '\0';

  // 送信
  // Serial1.print(send_str);


  // テスト
  Serial.print(threshold1*4);
  Serial.print("\t");
  Serial.print(threshold2*4);
  Serial.print("\t");
  for(int i=0;i<LINE_NUM;i++){
    Serial.print(line[i]);
    if((i+1)%5==0) Serial.print(" ");
  }
  Serial.print("\t\t");
  for(int i=0;i<ANALOG_NUM;i++){
    Serial.print(analogRead(ANALOG_PIN[i]));
    Serial.print("\t");
  }
  Serial.println();


  delay(4);
  // delay(3);
  // delay(4);
  // static uint32_t time = 0;
  // uint32_t t = millis() - time;
  // Serial.print(" ");
  // Serial.println(t);
  // time = millis();
}
