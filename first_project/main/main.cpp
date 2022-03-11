#include "Arduino.h"
#include "WiFi.h"
#include "Wire.h"

#include "dht.h"

#define ESP_NAME "esp32_temprature_sensor"

// Set your Static IP address
IPAddress local_IP(192, 168, 1, 200);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);

void initWiFi()
{
    if (!WiFi.config(local_IP, gateway, subnet)) {
        Serial.println("STA Failed to configure");
    }
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(ESP_NAME);
    WiFi.begin("luluvroumette", "coucoucestmoi");
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

#define DHTTYPE DHT20
DHT dht(DHTTYPE);

void setup()
{
    Serial.begin(115200);
    initWiFi();

    // I2C related init
    Wire.begin(); // maybe change pin

    dht.begin();
}

unsigned long previousMillis = 0;

void loop()
{
    unsigned long currentMillis = millis();
    // if WiFi is down, try reconnecting
    if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >= 30000)) {
        Serial.print(millis());
        Serial.println("Reconnecting to WiFi...");
        WiFi.disconnect();
        WiFi.reconnect();
        previousMillis = currentMillis;
    }

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
    } else
    {
        Serial.println("Failed to get temprature and humidity value.");
    }
    sleep(10);
}