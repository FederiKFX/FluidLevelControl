#include <WiFi.h>
#include <EEPROM.h>
#include <PubSubClient.h>
#include <ArduinoComponents.h>
#include <ArduinoJson.h>
#include <StreamUtils.h>

using namespace components;

const char* ssid = "Yatskiv";
const char* password = "password9632";

const char* mqtt_server = "192.168.1.128";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];

struct Device 
{
    int id;
    Vector<int> name;
    int fluidType;
    int fullness;
    int error;
    Vector<int> sensors;
    Vector<bool> pinsState;
    Vector<int> pinsNum;

    int follow_id;
    int follow_comparison;
    int follow_fullness;
    Vector<bool> pinsStateConf;
};

Device dev;

void setup() {
  Serial.begin(115200);
  EEPROM.begin(1024);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
  Serial.println("Reading EEPROM");
 
  DynamicJsonDocument json(1024);
  EepromStream eepromStream(0, 1024);
  DeserializationError error = deserializeJson(json, eepromStream);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  serializeJson(json, Serial);
  dev.fluidType = json["fluidType"];
  for(unsigned int i = 0; i < json["name"].size(); ++i)
  {
    dev.name.push(json["name"][i]);
  }
  dev.follow_id = json["follow_id"];
  dev.follow_comparison = json["follow_comparison"];
  dev.follow_fullness = json["follow_fullness"]; 
  for(unsigned int i = 0; i < json["pins_state"].size(); ++i)
  {
    dev.pinsStateConf.push(json["pins_state"][i]);
  }

  dev.id = 50;
  dev.sensors.push(32);
  dev.sensors.push(33);
  dev.sensors.push(25);
  dev.sensors.push(26);
  dev.sensors.push(27);
  dev.sensors.push(14);
  dev.sensors.push(12);
  dev.sensors.push(13);

  for(size_t i = 0; i < dev.sensors.size(); ++i)
  {
    pinMode(dev.sensors.get(i), INPUT_PULLUP );   
  }

  dev.pinsState.push(0);

  dev.pinsNum.push(18);

  for(size_t i = 0; i < dev.pinsNum.size(); ++i)
  {
    pinMode(dev.pinsNum.get(i), OUTPUT);
    digitalWrite(dev.pinsNum.get(i), dev.pinsState.get(i));   
  }
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, messageTemp);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
    
  if (String(topic) == "SetupInfo/Dev" + String(dev.id)) {
    Serial.println("Changing info");
    dev.fluidType = doc["fluidType"];
    dev.name.clear();
    for(unsigned int i = 0; i < doc["name"].size(); ++i)
    {
      dev.name.push(doc["name"][i]);
    }
  }
  if (String(topic) == "Setup/Dev" + String(dev.id)) {
    Serial.println("Changing setup");
    dev.follow_id = doc["follow_id"];
    dev.follow_comparison = doc["follow_comparison"];
    dev.follow_fullness = doc["follow_fullness"]; 
    dev.pinsStateConf.clear();
    for(unsigned int i = 0; i < doc["pins_state"].size(); ++i)
    {
      dev.pinsStateConf.push(doc["pins_state"][i]);
    }
    client.subscribe(("Status/Dev" + String(dev.follow_id)).c_str());
  }
  if (String(topic) == "Status/Dev" + String(dev.follow_id)) {
    Serial.println("Changing pins");
    bool comp = 0;
    if (dev.follow_comparison)
    {
      comp = doc["fullness"] > dev.follow_fullness;
    }
    else
    {
      comp = doc["fullness"] < dev.follow_fullness;
    }
    if(comp)
    {
      for(unsigned int i = 0; i < dev.pinsStateConf.size(); ++i)
      {
        dev.pinsState.get(i) = dev.pinsStateConf.get(i);
        digitalWrite(dev.pinsNum.get(i), dev.pinsState.get(i));
      }
    }
    else
    {
      for(unsigned int i = 0; i < dev.pinsState.size(); ++i)
      {
        dev.pinsState.get(i) = 0;
        digitalWrite(dev.pinsNum.get(i), dev.pinsState.get(i));
      }
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(("Dev" + String(dev.id)).c_str())) {
      Serial.println("connected");
      // Subscribe
      client.subscribe(("Setup/Dev" + String(dev.id)).c_str());
      client.subscribe(("SetupInfo/Dev" + String(dev.id)).c_str());
      client.subscribe(("Status/Dev" + String(dev.follow_id)).c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;

    DynamicJsonDocument json(1024);
    json["id"] = dev.id;
    for(unsigned int i = 0; i < dev.name.size(); ++i)
    {
      json["name"][i] = dev.name.get(i);
    }
    json["fluidType"] = dev.fluidType;

    int numOfOn = 0;
    bool check = true;
    for(unsigned int i = 0; i < dev.sensors.size(); ++i)
    {
      bool sensor = !digitalRead(dev.sensors.get(i));
      json["sensors"][i] = sensor;
      if (sensor && check)
        {
            numOfOn++;
        }
        else
        {
            check = false;
        }
    }
    dev.fullness = (float)numOfOn / dev.sensors.size() * 100;
    json["fullness"] = dev.fullness;
    
    for(unsigned int i = 0; i < dev.pinsState.size(); ++i)
    {
      json["pins"][i] = dev.pinsState.get(i);
    }

    json["follow_id"] = dev.follow_id;
    json["follow_comparison"] = dev.follow_comparison;
    json["follow_fullness"] = dev.follow_fullness;
    for(unsigned int i = 0; i < dev.pinsStateConf.size(); ++i)
    {
      json["pins_state"][i] = dev.pinsStateConf.get(i);
    }
    
    size_t size = measureJson(json) + 5;
    char* tempString = new char[size];
    serializeJson(json, tempString, size);
    
    EepromStream eepromStream(0, 1024);
    serializeJson(json, eepromStream);
    eepromStream.flush();
    
    client.publish(("Status/Dev" + String(dev.id)).c_str(), tempString);

    delete tempString;
  }
}
