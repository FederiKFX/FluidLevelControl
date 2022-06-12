#include <iostream>
#include <Windows.h>

#include "Info/DeviceInfo.h"

void on_connect(struct mosquitto* mosq, void* obj, int rc) {
    //printf("ID: %d\n", *(int*)obj);
    std::cout << toUtf8(static_cast<Device*>(obj)->name) << std::endl;
    if (rc) {
        printf("Error with result code: %d\n", rc);
        exit(-1);
    }
    //mosquitto_subscribe(mosq, NULL, "Status/#", 0);
    //mosquitto_subscribe(mosq, NULL, "Setup/#", 0);
}

void on_message(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg) {

    char** topics = new char* [10];
    for (int i = 0; i < 10; i++) {
        topics[i] = new char[20];
    }

    int count;

    /*if (mosquitto_sub_topic_tokenise(msg->topic, &topics, &count) == MOSQ_ERR_SUCCESS)
    {
        if (std::string(topics[1]) == toUtf8(static_cast<Device*>(obj)->name))
        {
            if (std::string(topics[0]) == "Status")
            {
                std::cout << toUtf8(static_cast<Device*>(obj)->name) << std::endl;
                printf("New message from %s: %s\n", topics[1], (char*)msg->payload);
            }
            if (std::string(topics[0]) == "Setup")
            {
                std::cout << toUtf8(static_cast<Device*>(obj)->name) << std::endl;
                printf("New config to %s: %s\n", topics[1], (char*)msg->payload);
            }
        }
    }*/
}

void DeviceSimulation(std::shared_ptr<Device> device)
{
    struct mosquitto* mosq;
    mosq = mosquitto_new(std::to_string(device->id).data(), true, device.get());

    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_message_callback_set(mosq, on_message);

    if (mosquitto_connect(mosq, "localhost", 1883, 10) == MOSQ_ERR_SUCCESS)
    {
        std::string msg1 = "Info";

        mosquitto_loop_start(mosq);
        Sleep(500);

        mosquitto_publish(mosq, NULL, ("Setup/Dev" + std::to_string(device->id)).data(), msg1.size(), msg1.c_str(), 0, false);

        nlohmann::json j = *(device.get());

        std::string msg = j.dump();
        mosquitto_publish(mosq, NULL, "Init", msg.size(), msg.c_str(), 0, false);
        std::cout << msg << std::endl;
        while (true)
        {
            mosquitto_publish(mosq, NULL, ("Status/Dev" + std::to_string(device->id)).data(), msg.size(), msg.c_str(), 0, false);
            Sleep(10000);
        }
        mosquitto_loop_stop(mosq, true);

        mosquitto_disconnect(mosq);
        mosquitto_destroy(mosq);
    }
}

int main()
{

// 	StateData data = { L"Tanнпk 1", Colour::GASOLINE, 53, { 1,1,1,1,0,0,0 } };
// 	nlohmann::json j;
// 
// 	j["name"] = data.name;
// 	j["type"] = data.fluidType;
// 	j["fulness"] = data.fullness;
// 
// 	std::cout << j.dump() << std::endl;
// 
// 	nlohmann::json t = nlohmann::json::parse(j.dump());
// 
// 	std::wstring f(t.items().begin().value().begin(), t.items().begin().value().end());
// 	//f.push_back(1085);
// 	//f.push_back(1087);
// 
// 	std::wstring h;
//     for (auto& [key, value] : t.items())
//     {
//         if (key == "name")
//         {
// 			std::wstring f(value.begin(), value.end());
//         }
//     }
// 	std::wcout << f << std::endl;
	//std::cout << t.items().begin().value() << std::endl;

    std::vector<std::shared_ptr<Device>> devices;
    devices.push_back(std::make_shared<Device>());
    devices.push_back(std::make_shared<Device>());
    devices[0]->sensors = { 1,1,1,1,1,0,0,0 };
    devices[0]->id = 54;
    devices[0]->name = L"Dev0Тесті";
    devices[0]->fluidType = Colour::GASOLINE;

    devices[1]->sensors = { 1,1,1,1,0,0 };
    devices[0]->id = 18;
    devices[1]->name = L"Dev1";
    devices[1]->fluidType = Colour::WATER;

    for (auto dev : devices)
    {
        int numOfOn = 0;
        for (auto sensor : dev->sensors)
        {
            if (sensor)
            {
                numOfOn++;
            }
            else
            {
                break;
            }
        }
        dev->fullness = (float)numOfOn / dev->sensors.size() * 100;
    }

    mosquitto_lib_init();

    std::thread thr1(DeviceSimulation, devices[0]);
    std::thread thr2(DeviceSimulation, devices[1]);

    //int res = WaitForSingleObject(thr1.native_handle(), INFINITE);
    thr1.detach();
    Sleep(1000);
    //devices[0]->name = L"Dev010";
    thr2.join();


    mosquitto_lib_cleanup();

    return 0;
}