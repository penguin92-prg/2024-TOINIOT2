#pragma once



#include<iostream>
#include<sstream>
#include<iomanip>
#include<condition_variable>
#include<mutex>

#include<algorithm>
#include<vector>

#include<winrt/windows.foundation.h>
#include<winrt/windows.foundation.collections.h>
#include<winrt/windows.devices.bluetooth.h>
#include<winrt/windows.devices.bluetooth.advertisement.h>
#include<winrt/windows.devices.bluetooth.genericattributeprofile.h>
#include<winrt/windows.devices.enumeration.h>
#include<winrt/windows.storage.streams.h>



using namespace std;

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Devices::Bluetooth;
using namespace Windows::Devices::Enumeration;
using namespace Windows::Devices::Bluetooth::Advertisement;
using namespace Windows::Devices::Bluetooth::GenericAttributeProfile;
using namespace Windows::Storage::Streams;



std::wstring UUIDToString(const winrt::guid& uuid);
std::wstring ServiceToString(const winrt::guid& uuid);
std::wstring getDeviceName(uint64_t addr);	// デバイス名を取得
bool sendWString(GattCharacteristic characteristic, wstring UUID, wstring send_str);	// 送信