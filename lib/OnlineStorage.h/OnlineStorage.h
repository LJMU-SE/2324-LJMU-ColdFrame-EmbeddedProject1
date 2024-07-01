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


    void tick(StorageManager* storageManager,Mode* currentMode){

        unsigned long currentMillis = millis();

        if (currentMillis - lastSent >= delayTime){
            sendData(storageManager->getAverages(),currentMode);
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
        http.begin(this->serverAdress);
        http.addHeader("Content-Type", "application/json");
        int response = http.POST(jsonData);
       

        String responseString = http.getString();

        Serial.printf("Readings sent; Time to send %ds; response code %d \n",(millis() - before) / 1000,response);
        Serial.println(responseString);
        
    }

    void connect(){
        // Connects to wifi and sends initial dummy values to server
        WiFi.begin(this->ssid, this->password);
        Serial.println("\nConnecting");

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
char* serverAdress = "https://api.ljmu.dev/cold-frame/push-latest";
long delayTime = 270000;
unsigned long lastSent = 0;


};




#endif
