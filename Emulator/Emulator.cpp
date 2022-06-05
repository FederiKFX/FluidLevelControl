#include <iostream>
#include <nlohmann/json.hpp>
#include <mosquitto.h>

#include "DataTypes.h"

std::string toUtf8(const std::wstring& utf16)
{
	if (utf16.empty())
	{
		return "";
	}

	std::string utf8;
	utf8.resize(utf16.size() * 2);
	int count = WideCharToMultiByte(CP_UTF8, 0, utf16.data(), static_cast<int>(utf16.size()), utf8.data(), static_cast<int>(utf8.size()), nullptr, nullptr);
	if (count == 0)
	{
		return "<unknown>";
	}
	utf8.resize(count);

	return utf8;
}

std::wstring toUtf16(const std::string& utf8)
{
	if (utf8.empty())
	{
		return L"";
	}

	std::wstring utf16;
	utf16.resize(utf8.size() * 2);
	int count = MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), utf16.data(), static_cast<int>(utf16.size()));
	if (count == 0)
	{
		return L"<unknown>";
	}
	utf16.resize(count);

	return utf16;
}

int main() {

	StateData data = { L"Tanнпk 1", FluidType::GASOLINE, 53, { 1,1,1,1,0,0,0 } };
	nlohmann::json j;

	j["name"] = data.name;
	j["type"] = data.fluidType;
	j["fulness"] = data.fullness;

	std::cout << j.dump() << std::endl;

	nlohmann::json t = nlohmann::json::parse(j.dump());

	std::wstring f(t.items().begin().value().begin(), t.items().begin().value().end());
	//f.push_back(1085);
	//f.push_back(1087);
	std::wcout << f << std::endl;
	//std::cout << t.items().begin().value() << std::endl;

	/*int rc;
	struct mosquitto* mosq;

	mosquitto_lib_init();

	mosq = mosquitto_new("publisher-test", true, NULL);

	rc = mosquitto_connect(mosq, "localhost", 1883, 60);
	if (rc != 0) {
		printf("Client could not connect to broker! Error Code: %d\n", rc);
		mosquitto_destroy(mosq);
		return -1;
	}
	printf("We are now connected to the broker!\n");

	mosquitto_publish(mosq, NULL, "test/t1", 6, "Hello", 0, false);

	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);

	mosquitto_lib_cleanup();*/
	return 0;
}