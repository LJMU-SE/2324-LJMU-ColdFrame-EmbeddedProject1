#include <Modes.h>
#include <Arduino.h>
#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

const int ELEMENT_COUNT_MAX = 60 * 24;

unsigned long storageLastChange = 0;
const long STORAGE_DELAY = 6000; //CHANGE BACK TO 60000

// Keeps track of debug values
struct Readings{
  int temp;
  int hum;
  int moist;
  String envState;
  String soilState;
};

class StorageManager{
public:

    StorageManager(){ 
        this->currentIndex = 0;
        this->humRange.max = 0;
        this->humRange.min = 100;
        this->tempRange.max = 0;
        this->tempRange.min = 100;
        this->storageFull = false;
    }

    void tick(){

        unsigned long currentMillis = millis();

        if (currentMillis - storageLastChange >= STORAGE_DELAY)
        {
            storageLastChange = currentMillis;
            
            if (this->currentIndex > ELEMENT_COUNT_MAX){
                this->currentIndex = 0;
                this->storageFull = true;
            }
            this->readings[this->currentIndex] = this->lastRead;
            updateMinMax();
            this->currentIndex++;
        }
    }

    void storeLastRead(Readings latest){
        this->lastRead = latest;
    }

    MinMax getMinMaxTemp(){
        return this->tempRange;
    }
    
    MinMax getMinMaxHum(){
        return this->humRange;
    }

private:
Readings readings[ELEMENT_COUNT_MAX];
Readings lastRead;
MinMax tempRange;
MinMax humRange;
uint8_t currentIndex;
boolean storageFull;

    void updateMinMax(){

        int length = storageFull ? ELEMENT_COUNT_MAX : currentIndex;

        for (int i = 0; i < length; i++){

            if (this->readings[i].temp > this->tempRange.max){
                this->tempRange.max = this->readings[i].temp;
            }

            if (this->readings[i].temp < this->tempRange.min){
                this->tempRange.min = this->readings[i].temp;
            }

            if (this->readings[i].hum > this->humRange.max){
                this->humRange.max = this->readings[i].hum;
            }

            if (this->readings[i].hum < this->humRange.min){
                this->humRange.min = this->readings[i].hum;
            }
        }
    }

};

#endif 
