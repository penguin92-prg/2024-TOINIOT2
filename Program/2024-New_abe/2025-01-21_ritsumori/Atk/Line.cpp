#include "Line.hpp"


void Line::begin(int rate){
  baudrate = rate;
  Serial1.begin(baudrate);

  return;
}


void Line::read(){
  // 参考リンク: https://note.com/shiokara_rcj/n/n44bbd2454c07

  // 必要な分のデータを受信していない場合処理を飛ばす
  if(Serial1.available()<STR_SIZE){
    return;
  }
    
  // 古い情報を読み飛ばす
  while(Serial1.available()>STR_SIZE){
    Serial1.read();
  }
  // 開始コードが来るまで読み飛ばす
  while(Serial1.available()){
    if(Serial1.read() == 0b00011111) break;
  }

  // 送信中の場合、完了するまで待つ
  while(Serial1.available()<STR_SIZE-2){int i=0;}



  // 読み出して格納
  for(int i=0;i<6;i++){
    char c = Serial1.read();
    for(int j=0;j<5;j++){
      line[i*5+j] = (c>>(4-j)) & 0b00000001;
    }
  }
  
  
  
  // エンジェルリングの反応している数を調べる
  num = 0;
  for(int i=0;i<INNER_NUM;i++){
    if(line[i]) num++;
  }
  // 外側が反応しているかを調べる
  front = line[INNER_NUM];
  left  = line[INNER_NUM+1];
  back  = line[INNER_NUM+2];
  right = line[INNER_NUM+3];



  // 論理和を用いて外側のいずれかが反応しているかを調べる
  outside = front | left | back | right;

  // エンジェルリングが反応しているかを調べる
  angel = num > 0;

  // 論理和を用いてラインセンサが反応しているかを調べる
  on = angel | outside;

  // 踏んでいない場合に処理をスキップ
  if(!on){
    return;
  }



  vec.clear();

  // ラインセンサの反応している部分をひとかたまりにしたとき、それぞれ
  int index = 0;        // 塊のインデックス
  Vec2 v[INNER_NUM];    // 塊のベクトルの平均
  int count[INNER_NUM]; // 塊に含まれている反応しているセンサの数
  
  // 初期化
  for(int i=0;i<INNER_NUM;i++){
    v[i].x = 0;
    v[i].y = 0;
    count[i] = 0;
  }


  // 連続する部分をひとまとめにする
  for(int i=0;i<INNER_NUM;i++){
    if(line[i]){
      float sensor_dir = radians(i*360/INNER_NUM);
      v[index].x += cos(sensor_dir);
      v[index].y += sin(sensor_dir);
      count[index]++;
    }

    // １つ前のセンサが反応していて今のセンサが反応していない場合、塊が切れたものとみなしてインデックスを加算し、次の塊の処理に備える
    if(line[(i+INNER_NUM-1)%INNER_NUM] && !line[i]){
      index++;
    }
  }

  // エンジェルリングのうち、最後に調べたセンサと0番目のセンサが反応しているとき、最後のセンサが含まれる塊を0番目の塊に合成するためのもの(全然ちゃんと書けてない)
  // if(line[INNER_NUM-1]){
  //   v[0].x += v[index].x;
  //   v[0].y += v[index].y;
  // }
  // 正しくは多分こう
  if(line[INNER_NUM-1] && line[0]){
    v[0] += v[index];
    count[0] += count[index];
    index -= 1;
  }


  // 塊の数
  area = index;

  
  // 塊のベクトルの平均を求め、白線の方向ベクトルに加算
  for(int i=0;i<index;i++){
    v[i] /= (float)count[i];
    vec += v[i];
  }


  // 外側のみ反応している場合、ここまでの処理を無視してそれぞれ真っ直ぐ避ける方向に動く
  if(!angel){
    if(front) vec.x = 1;
    if(left)  vec.y = 1;
    if(back)  vec.x = -1;
    if(right) vec.y = -1;
  }



  // 角度・距離を算出
  dir_prev = dir;
  dir = -degrees(atan2(vec.y, vec.x));
  if(index == 0) index = 1;            // 0除算防止
  distance = vec.len() / (float)index; // ラインとの距離を算出

  // 踏み始めの場合、dir_prevをdirにする
  if(prev_on == false && on == true){
    dir_prev = dir;
  }

  // 1ループ前から30度以上変わっていた場合にdir_prevをdirにする (白線を超えたときのための処理)
  float diff = abs(dir - dir_prev);
  if(diff > 30){
    dir = dir_prev;
  }



  return;
}


void Line::send(char command){
  Serial1.print(command);
  return;
}
