#pragma once

#include<iostream>
#include<sstream>
#include<iomanip>
#include<condition_variable>
#include<mutex>

#include<Windows.h>
#include<winrt/windows.foundation.h>
#include<winrt/windows.foundation.collections.h>
#include<winrt/windows.devices.bluetooth.h>
#include<winrt/windows.devices.bluetooth.advertisement.h>
#include<winrt/windows.devices.bluetooth.genericattributeprofile.h>
#include<winrt/windows.devices.enumeration.h>
#include<winrt/windows.storage.streams.h>

#include<algorithm>
#include<vector>

#include"WstringToString.hpp"


using namespace std;

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Devices::Bluetooth;
using namespace Windows::Devices::Enumeration;
using namespace Windows::Devices::Bluetooth::Advertisement;
using namespace Windows::Devices::Bluetooth::GenericAttributeProfile;
using namespace Windows::Storage::Streams;



extern bool is_found;
extern uint64_t esp_addr;

struct Deviceinfo_t {
	wstring name = L"";
	uint64_t uuid = 0;
	bool is_connectable = false;
	int strength = -1;
};

extern vector<Deviceinfo_t> device_list;
extern vector<uint64_t> addr_list;



std::wstring UUIDToString(const winrt::guid& uuid);
wstring getDeviceName(uint64_t addr);
void onReceived(BluetoothLEAdvertisementWatcher watcher, BluetoothLEAdvertisementReceivedEventArgs eventArgs);
bool sendWString(GattCharacteristic characteristic, wstring UUID, wstring send_str);	// ëóêM
