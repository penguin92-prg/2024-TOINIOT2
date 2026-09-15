#include<stdlib.h>
#include<Windows.h>

#include "general.hpp"

/*

	UUID etc...
	#define CENTRAL_NAME "TOINIOT2_CONTROLLER"
	#define SERVICE_UUID "6a10c527-bac9-485e-bdb8-462f3b72bd59"
	#define NOTIFY_UUID "07edec9a-716c-4379-b18b-2bb8eb1a2bea"
	#define CIPO_UUID "d4b0e22e-2597-48e3-886e-ea984f3f7db0"
	#define COPI_UUID "2234bf79-5ff8-4b47-a139-b9a20801eaf4"

*/

/*

	送信

	0: Lスティック左右
	1: Lスティック上下
	2: Rスティック左右
	3: Rスティック上下
	4: ボタン (キッカー / 姿勢リセット)

*/


bool	 is_found = false;
uint64_t esp_addr = 260422049255626;


// 受信時の動作
vector<uint64_t> addr_list(0);

void onReceived(BluetoothLEAdvertisementWatcher watcher, BluetoothLEAdvertisementReceivedEventArgs eventArgs) {
	//cout << "Received Advertisement!" << endl;

	auto addr = eventArgs.BluetoothAddress();
	if (addr_list.empty() || !(find(addr_list.begin(), addr_list.end(), addr) != addr_list.end()) && eventArgs.RawSignalStrengthInDBm() > -80) {
		if (addr == esp_addr) {
			cout << "esp32 found!" << endl;
		}


		// デバイス名
		cout << "(" << addr_list.size() << ") ";
		if (addr == esp_addr) {
			wcout << getDeviceName(addr) << endl;
		}
		else {
			cout << endl;
		}


		// 基本情報
		cout << "  addr        :" << addr << endl;
		cout << "  connectable?:" << eventArgs.IsConnectable() << endl;
		cout << "  strength    :" << eventArgs.RawSignalStrengthInDBm() << "(dbm)" << endl;


		cout << endl;
		addr_list.push_back(addr);


		if (addr == esp_addr) {
			is_found = true;
		}
	}


}


int main() {
	cout << "pid:" << this_thread::get_id() << endl;
	cout << "initializing..." << endl;
	init_apartment();


	// スキャン準備
	BluetoothLEAdvertisementWatcher watcher;
	watcher.Received(&onReceived);	// 受信時の動作を設定



	// アドバタイズメントをスキャン
	watcher.Start();				// 受信を開始 Enterが押されるまで続ける
	while (is_found == false);
	watcher.Stop();



	// 通信
	auto device = BluetoothLEDevice::FromBluetoothAddressAsync(esp_addr).get();
	auto result = device.GetGattServicesAsync(BluetoothCacheMode::Uncached).get();

	IVectorView<GattCharacteristic> characteristics;



	// 成功時
	if (result.Status() == GattCommunicationStatus::Success) {
		auto services = result.Services();

		// サービスを取得
		for (auto service : services) {
			auto uuid = service.Uuid();
			wcout << endl << L"  Service:" << ServiceToString(uuid) << endl;

			// キャラクタリスティックを取得
			auto result = service.GetCharacteristicsAsync().get();
			if (result.Status() == GattCommunicationStatus::Success) {
				characteristics = result.Characteristics();

				for (auto characteristic : characteristics) {
					// 格納
					//characteristics.emplace_back(&characteristic);

					// UUIDを表示
					//auto uuid = characteristic.Uuid();
					//wcout << L"  Characteristic:" << ServiceToString(uuid) << endl;

				}
			}


		}
	}
	else {
		cerr << "Failed to connect" << endl;
	}

	system("cls");
	for (const auto& c : characteristics) {
		auto uuid = c.Uuid();
		wcout << L"  Characteristic:" << ServiceToString(uuid) << endl;
	}

	cout << "begin writing" << endl;



	int val = 0;
	while (getchar() != ' ') {

		for (auto characteristic : characteristics) {
			if (sendWString(characteristic, L"D4B0E22E-2597-48E3-886E-EA984F3F7DB0", L"FUJIKI" + to_wstring(val))) {
				val++;
			}

		}

	}

	// 通信を終了
	device.Close();



	return 0;

}