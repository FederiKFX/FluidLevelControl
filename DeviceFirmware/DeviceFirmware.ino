#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoComponents.h>
#include <ArduinoJson.h>

using namespace components;

const char* ssid = "Yatskiv";
const char* password = "password9632";

const char* mqtt_server = "192.168.1.127";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];

struct Device 
{
    uint64_t id;
    Vector<int> name;
    int fluidType;
    uint32_t fullness;
    Vector<bool> sensors;
    Vector<bool> pins; 
};

Device dev;

void setup() {
  Serial.begin(115200);
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  dev.id = 50;
  dev.sensors.push(1);
  dev.sensors.push(1);
  dev.sensors.push(0);
  dev.sensors.push(1);
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
  /*if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(ledPin, LOW);
    }
  }*/
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("Dev50")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("Setup/#");
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

  dev.fluidType = 11;

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
      bool sensor = dev.sensors.get(i);
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
    
    for(unsigned int i = 0; i < dev.pins.size(); ++i)
    {
      json["pins"][i] = dev.pins.get(i);
    }

    
    size_t size = measureJson(json) + 5;
    char* tempString = new char[size];
    serializeJson(json, tempString, size);
    client.publish("Status/Dev50", tempString);

    delete tempString;
  }
}
