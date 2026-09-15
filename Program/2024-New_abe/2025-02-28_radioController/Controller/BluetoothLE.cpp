#include"BluetoothLE.hpp"



bool is_found = false;
uint64_t esp_addr = 260422049255626;

vector<Deviceinfo_t> device_list(0);
vector<uint64_t> addr_list(0);





std::wstring UUIDToString(const winrt::guid& uuid)
{
	std::wstringstream str;
	str << std::uppercase << std::hex;
	str << std::setw(8) << std::setfill(L'0') << uuid.Data1 << L"-";
	str << std::setw(4) << std::setfill(L'0') << uuid.Data2 << L"-";
	str << std::setw(4) << std::setfill(L'0') << uuid.Data3 << L"-";
	str << std::setw(2) << std::setfill(L'0') << static_cast<short>(uuid.Data4[0])
		<< std::setw(2) << std::setfill(L'0') << static_cast<short>(uuid.Data4[1])
		<< '-'
		<< std::setw(2) << std::setfill(L'0') << static_cast<short>(uuid.Data4[2])
		<< std::setw(2) << std::setfill(L'0') << static_cast<short>(uuid.Data4[3])
		<< std::setw(2) << std::setfill(L'0') << static_cast<short>(uuid.Data4[4])
		<< std::setw(2) << std::setfill(L'0') << static_cast<short>(uuid.Data4[5])
		<< std::setw(2) << std::setfill(L'0') << static_cast<short>(uuid.Data4[6])
		<< std::setw(2) << std::setfill(L'0') << static_cast<short>(uuid.Data4[7]);
	str << std::nouppercase;
	return str.str();

}

// デバイス名を取得
wstring getDeviceName(uint64_t addr) {
	auto dev = BluetoothLEDevice::FromBluetoothAddressAsync(addr).get();
	if (dev == nullptr) {
		return L"Unnamed";
	}


	// これやると次のキャラクテリスティックの取得が早くなる（全部の情報をここで取得している？）
	auto gapServicesResult = dev.GetGattServicesForUuidAsync(GattServiceUuids::GenericAccess(), BluetoothCacheMode::Uncached).get();
	if (gapServicesResult.Status() == GattCommunicationStatus::Success) {
		IVectorView<GattDeviceService> gapServices = gapServicesResult.Services();
		if (gapServices.Size() > 0) {
			GattDeviceService genericAccessSvc = gapServices.GetAt(0);
			if (genericAccessSvc) {
				IVectorView<GattCharacteristic> gapDeviceNameChrs = genericAccessSvc.GetCharacteristics(GattCharacteristicUuids::GapDeviceName());
				if (gapDeviceNameChrs.Size() == 1) {
					GattCharacteristic gapDeviceNameChr = gapDeviceNameChrs.GetAt(0);

					GattReadResult readRes = gapDeviceNameChr.ReadValueAsync().get();
					if (readRes.Status() == GattCommunicationStatus::Success) {
						DataReader reader = DataReader::FromBuffer(readRes.Value());
						return reader.ReadString(reader.UnconsumedBufferLength()).c_str();
					}
				}
			}
		}
	}

	wstring name = dev.Name().c_str();
	if (name.empty()) {
		name = L"Unnamed Device";
	}

	return name;
}


// 受信時の動作
void onReceived(BluetoothLEAdvertisementWatcher watcher, BluetoothLEAdvertisementReceivedEventArgs eventArgs) {
	auto addr = eventArgs.BluetoothAddress();
	if (addr_list.empty() || !(find(addr_list.begin(), addr_list.end(), addr) != addr_list.end()) && eventArgs.RawSignalStrengthInDBm() > -80) {

		Deviceinfo_t device;

		// デバイス名
		//device.name = getDeviceName(addr);

		// 基本情報
		device.uuid = addr;
		device.is_connectable = eventArgs.IsConnectable();
		device.strength = eventArgs.RawSignalStrengthInDBm();


		device_list.push_back(device);
		addr_list.push_back(addr);
		
		if (device.uuid == esp_addr) {
			is_found = true;
		}

	}


}


// 送信
bool sendWString(GattCharacteristic characteristic, wstring UUID, wstring send_str) {

	if (UUIDToString(characteristic.Uuid()) == UUID) {
		DataWriter writer;
		writer.WriteString(send_str);

		auto status = characteristic.WriteValueWithResultAsync(writer.DetachBuffer()).get();
		if (status.Status() == GattCommunicationStatus::Success) {
			wcout << UUIDToString(characteristic.Uuid()) << L"    wrote: " << send_str;
		}

		return true;
	}

	return false;

}