#include "DxLib.h"

#include<sstream>

#include "BluetoothLE.hpp"
#include "General.hpp"
#include "GamePad.hpp"
#include "WstringToString.hpp"


using namespace std;



int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	SetOutApplicationLogValidFlag(TRUE);	// output log
	ChangeWindowMode(TRUE);					//ウィンドウモード切替
	SetGraphMode(WINDOW_W, WINDOW_H, 32);	//ウィンドウサイズ決める
	SetMainWindowText("Radio Controller");	//ソフト名決める
	SetWindowStyleMode(7);					//画面最大化できるようにする
	SetWindowSizeChangeEnableFlag(TRUE);	//ウィンドウサイズ変えれるようにする
	SetAlwaysRunFlag(TRUE);					//ソフトがアクティブじゃなくても処理続行するようにする
	SetWindowSizeExtendRate(1.0f);			//起動時のウィンドウサイズを設定 ( 1 = 100%)
	SetUseMaskScreenFlag(TRUE);				//書かなくても良い。マスク使うときだけ書こう


	init_apartment(apartment_type::multi_threaded);	// 初期化
	if (DxLib_Init() == -1) {
		return -1;
	}

	SetFontSize(font_size);




	// スキャン準備
	BluetoothLEAdvertisementWatcher watcher;
	watcher.Received(&onReceived);	// 受信時の動作を設定


	// アドバタイズメントをスキャン
	watcher.Start();				// 受信を開始 Enterが押されるまで続ける
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_RETURN) == FALSE && CheckHitKey(KEY_INPUT_ESCAPE) == FALSE && is_found == false) {
		//ClearDrawScreen();
		//DrawBox(0, 0, WINDOW_W, WINDOW_H, gray, TRUE);

		//DrawString(8, 8, "Scanning advertisement...", white);
		//wstring str = L"";

		//for (const auto& device : device_list) {

		//	//str += "\nname: ";
		//	//str += device.name;
		//	str += L"\n  uuid: ";
		//	str += to_wstring(device.uuid);
		//	str += L"\n  connectable: ";
		//	str += to_wstring(device.is_connectable);
		//	str += L"\n  strength: ";
		//	str += to_wstring(device.strength);
		//	str += L"\n\n";
		//}

		//DrawString(8, 64, WstringToString(str).c_str(), white);

		//ScreenFlip();
	}
	watcher.Stop();



	// 通信
	auto device = BluetoothLEDevice::FromBluetoothAddressAsync(esp_addr).get();	// err
	auto result = device.GetGattServicesAsync(BluetoothCacheMode::Uncached).get();
	IVectorView<GattCharacteristic> characteristics;

	// 接続成功時
	if (result.Status() == GattCommunicationStatus::Success) {
		auto services = result.Services();

		// サービスを取得
		for (auto service : services) {
			auto uuid = service.Uuid();
			//wcout << endl << L"  Service:" << ServiceToString(uuid) << endl;
			//service

			// キャラクタリスティックを取得
			auto result = service.GetCharacteristicsAsync().get();
			if (result.Status() == GattCommunicationStatus::Success) {
				characteristics = result.Characteristics();
			}

		}
	}
	else {
		DxLib_End();
		return -1;
	}



	SetDrawScreen(DX_SCREEN_BACK);
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == FALSE) {
		// input
		static auto prev_time = GetNowCount();

		auto num = GetJoypadNum();
		auto input = GetJoypadInputState(DX_INPUT_KEY_PAD1);

		DINPUT_JOYSTATE dinput;
		GetJoypadDirectInputState(DX_INPUT_PAD1, &dinput);

		Stick L{ dinput.X, dinput.Y }, R{ dinput.Rx, dinput.Ry };
		bool boost = (bool)(input & BUTTON_L);
		bool kick = (bool)(input & BUTTON_R);
		bool reset = (bool)(input & BUTTON_Y);

		if (num == 0) {
			L.x = CheckHitKey(KEY_INPUT_A) ? -1000 : L.x;
			L.x = CheckHitKey(KEY_INPUT_D) ? 1000 : L.x;
			L.y = CheckHitKey(KEY_INPUT_W) ? -1000 : L.y;
			L.y = CheckHitKey(KEY_INPUT_S) ? 1000 : L.y;
			R.y = CheckHitKey(KEY_INPUT_LEFT) ? -1000 : R.y;
			R.y = CheckHitKey(KEY_INPUT_RIGHT) ? 1000 : R.y;
			boost = CheckHitKey(KEY_INPUT_LSHIFT) ? true : false;
			kick = CheckHitKey(KEY_INPUT_SPACE) ? true : false;
			reset = CheckHitKey(KEY_INPUT_R) ? true : false;
		}



		// compute

		// 送信する文字列の準備
		// -1024~1024 (11) -> -128 ~ 128 (8)
		stringstream ss;
		ss << L.x << " " << L.y << " " << R.x << " " << R.y << " " << boost << " " << kick << " " << reset;

		string send_str = ss.str();

		for (auto characteristic : characteristics) {
			sendWString(characteristic, L"D4B0E22E-2597-48E3-886E-EA984F3F7DB0", StringToWstring(send_str));
		}



		// draw	
		ClearDrawScreen();
		DrawBox(0, 0, WINDOW_W, WINDOW_H, gray, TRUE);

		//DrawLineAA(0, CENTER_Y, WINDOW_W, CENTER_Y, blue);
		//DrawLineAA(CENTER_X, 0, CENTER_X, WINDOW_H, blue);

		// gamepad
		DrawGamepadScreen(num, input, L, R);


		// send str
		string draw_str = "";
		for (auto c : send_str) {
			for (int i = 0; i < sizeof(c) * 8; i++) {
				if ((c >> (8 - 1 - i)) & 1) {
					draw_str += "1";
				}
				else {
					draw_str += "0";
				}
			}
			draw_str += " ";
		}

		DrawString(8, WINDOW_H - 8 - font_size, draw_str.c_str(), white);

		// frame rate
		double frame_rate = 1000.0 / (GetNowCount() - prev_time);
		DrawFormatString(WINDOW_W - 8 - GetDrawFormatStringWidth("%d fps", (int)frame_rate), 8, white, "%d fps", (int)frame_rate);
		prev_time = GetNowCount();



		ScreenFlip();
	}

	device.Close();
	DxLib_End();    // DXライブラリ終了処理

	return 0;
}
