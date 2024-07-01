#ifndef ONLINESTORAGE_H
#define ONLINESTORAGE_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <StorageManager.h>


class OnlineStorage {
    public:
            OnlineStorage(char* _ssid, char* _password){
            this->ssid = _ssid;
            this->password = _password;
            connect();
        }

        void tick(Readings latest,Mode* currentMode){
            unsigned long currentMillis = millis();

            // Sends initial readings on start up
            if (numSends < 1)
                sendData(latest,currentMode);

            if (currentMillis - lastSent >= delayTime){
                sendData(latest,currentMode);
                lastSent = currentMillis;
            }
        }


    private:
        void sendData(Readings latest,Mode* currentMode){
            StaticJsonDocument<200> doc;

            doc["temperature"] = latest.temp;
            doc["humidity"] = latest.hum;
            doc["moisture"] = latest.moist;
            doc["environmentState"] = latest.envState;
            doc["soilState"] = latest.soilState;
            doc["mode"] = currentMode->getModeString();

            String jsonData;
            serializeJson(doc, jsonData);

            Serial.print("Data Sending : ");
            Serial.println(jsonData);

            unsigned long before = millis();

            HTTPClient http;
            http.begin(this->serverAddress);
            http.addHeader("Content-Type", "application/json");
            int response = http.POST(jsonData);
            numSends += 1;

            String responseString = http.getString();

            Serial.printf("Readings sent; Time to send %ds; response code %d \n",(millis() - before) / 1000,response);
            Serial.println(responseString);
        }

        void connect(){
            // Output the MAC Address of the client
            byte mac[6];
            WiFi.macAddress(mac);
            Serial.print("\nMAC ADDRESS: ");
            Serial.print(mac[5],HEX);
            Serial.print(":");
            Serial.print(mac[4],HEX);
            Serial.print(":");
            Serial.print(mac[3],HEX);
            Serial.print(":");
            Serial.print(mac[2],HEX);
            Serial.print(":");
            Serial.print(mac[1],HEX);
            Serial.print(":");
            Serial.println(mac[0],HEX);
            
            WiFi.begin(this->ssid, this->password);
            Serial.println("Connecting to " + String(this->ssid));

            while(WiFi.status() != WL_CONNECTED){
                Serial.print(".");
                delay(100);
            }

            Serial.println("\nConnected to WiFi");
            Serial.print("ESP32 IP: ");
            Serial.println(WiFi.localIP());
        }

        char* ssid;
        char* password;
        char* serverAddress = "https://api.ljmu.dev/cold-frame/push-latest";
        long delayTime = 300000;
        unsigned long lastSent = 0;
        int numSends = 0;
};

#endif