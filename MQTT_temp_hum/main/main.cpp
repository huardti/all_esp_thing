#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>

#include "PubSubClient.h"
#include "dht.h"

#define ESP_NAME "esp32_temprature_sensor"

// Set your Static IP address
IPAddress local_IP(192, 168, 1, 200);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);

// Replace the next variables with your SSID/Password combination
const char* ssid = "luluvroumette";
const char* password = "coucoucestmoi";

// Add your MQTT Broker IP address, example:
const char* mqtt_server = "192.168.1.100";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void initWiFi()
{
    if (!WiFi.config(local_IP, gateway, subnet)) {
        Serial.println("STA Failed to configure");
    }
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(ESP_NAME);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
    }
    Serial.println(WiFi.localIP());

    WiFi.setSleep(WIFI_PS_MAX_MODEM);

}

void i2c_scan()
{
    Serial.println("\nI2C Scanner");
    byte error, address;
    int nDevices;
    Serial.println("Scanning...");
    nDevices = 0;
    for(address = 1; address < 127; address++ ) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0) {
        Serial.print("I2C device found at address 0x");
        if (address<16) {
            Serial.print("0");
        }
        Serial.println(address,HEX);
        nDevices++;
        }
        else if (error==4) {
        Serial.print("Unknow error at address 0x");
        if (address<16) {
            Serial.print("0");
        }
        Serial.println(address,HEX);
        }
    }
    if (nDevices == 0) {
        Serial.println("No I2C devices found\n");
    }
    else {
        Serial.println("done\n");
    }
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
  if (String(topic) == "esp32/state") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
    }
    else if(messageTemp == "off"){
      Serial.println("off");
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/subscribed");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

#define DHTTYPE DHT20
DHT dht(DHTTYPE);

void setup()
{
    Serial.begin(115200);
    initWiFi();

    // I2C related init
    Wire.begin(); // maybe change pin

    dht.begin();

    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

unsigned long previousMillis = 0;

void loop()
{
    unsigned long currentMillis = millis();
    // if WiFi is down, try reconnecting
    if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >= 30000))
    {
        Serial.print(millis());
        Serial.println("Reconnecting to WiFi...");
        WiFi.disconnect();
        WiFi.reconnect();
        previousMillis = currentMillis;
    }

    if (!client.connected())
    {
        reconnect();
    }
    client.loop();




    float temp_hum_val[2] = {0};

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    if (!dht.readTempAndHumidity(temp_hum_val))
    {
        Serial.print("Humidity: ");
        Serial.print(temp_hum_val[0]);
        Serial.print(" %\t");
        Serial.print("Temperature: ");
        Serial.print(temp_hum_val[1]);
        Serial.println(" *C");

        char humString[8];
        dtostrf(temp_hum_val[0], 1, 2, humString);
        client.publish("esp32/humidity", humString);

        char tempString[8];
        dtostrf(temp_hum_val[1], 1, 2, tempString);
        client.publish("esp32/temperature", tempString);

    }
    else
    {
        Serial.println("Failed to get temprature and humidity value.");
    }
    sleep(5*60);
}
