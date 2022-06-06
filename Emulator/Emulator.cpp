#include <iostream>
#include <nlohmann/json.hpp>
#include <mosquitto.h>

#include "DataTypes.h"

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

	std::wstring h;
    for (auto& [key, value] : t.items())
    {
        if (key == "name")
        {
			std::wstring f(value.begin(), value.end());
        }
    }
	std::wcout << f << std::endl;
	//std::cout << t.items().begin().value() << std::endl;

	int rc;
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

	std::string msg = j.dump();
	mosquitto_publish(mosq, NULL, "test/t1", msg.size(), msg.c_str(), 0, false);
	Sleep(500);

	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);

	mosquitto_lib_cleanup();
	return 0;
}