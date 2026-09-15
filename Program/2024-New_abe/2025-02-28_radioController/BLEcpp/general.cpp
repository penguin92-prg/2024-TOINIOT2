#include "general.hpp"


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

std::wstring ServiceToString(const winrt::guid& uuid)
{
	if (uuid == GattServiceUuids::AlertNotification())
	{
		return L"Alert Notification";
	}
	if (uuid == GattServiceUuids::Battery())
	{
		return L"Battery";
	}
	if (uuid == GattServiceUuids::BloodPressure())
	{
		return L"Blood Pressure";
	}
	if (uuid == GattServiceUuids::CurrentTime())
	{
		return L"Current Time";
	}
	if (uuid == GattServiceUuids::CyclingPower())
	{
		return L"Cycling Power";
	}
	if (uuid == GattServiceUuids::CyclingSpeedAndCadence())
	{
		return L"Cycling Speed and Cadence";
	}
	if (uuid == GattServiceUuids::DeviceInformation())
	{
		return L"Device Information";
	}
	if (uuid == GattServiceUuids::GenericAccess())
	{
		return L"Generic Access";
	}
	if (uuid == GattServiceUuids::GenericAttribute())
	{
		return L"Generic Attribute";
	}
	if (uuid == GattServiceUuids::Glucose())
	{
		return L"Glucose";
	}
	if (uuid == GattServiceUuids::HealthThermometer())
	{
		return L"Health Thermometer";
	}
	if (uuid == GattServiceUuids::HeartRate())
	{
		return L"Heart Rate";
	}
	if (uuid == GattServiceUuids::HumanInterfaceDevice())
	{
		return L"Human Interface Device";
	}
	if (uuid == GattServiceUuids::ImmediateAlert())
	{
		return L"Immediate Alert";
	}
	if (uuid == GattServiceUuids::LinkLoss())
	{
		return L"Link Loss";
	}
	if (uuid == GattServiceUuids::LocationAndNavigation())
	{
		return L"Location and Navigation";
	}
	if (uuid == GattServiceUuids::NextDstChange())
	{
		return L"Next Dst Change";
	}
	if (uuid == GattServiceUuids::PhoneAlertStatus())
	{
		return L"Phone Alert Status";
	}
	if (uuid == GattServiceUuids::ReferenceTimeUpdate())
	{
		return L"Reference Time Update";
	}
	if (uuid == GattServiceUuids::RunningSpeedAndCadence())
	{
		return L"Running Speed and Cadence";
	}
	if (uuid == GattServiceUuids::ScanParameters())
	{
		return L"Scan Parameters";
	}
	if (uuid == GattServiceUuids::TxPower())
	{
		return L"Tx Power";
	}
	return UUIDToString(uuid);
}

std::wstring getDeviceName(uint64_t addr) {
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

bool sendWString(GattCharacteristic characteristic, wstring UUID, wstring send_str) {

	if (UUIDToString(characteristic.Uuid()) == UUID){
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