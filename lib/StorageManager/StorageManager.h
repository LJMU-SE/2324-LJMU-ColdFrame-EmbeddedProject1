#include <ModeManager.h>
#include <Arduino.h>
#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

const int ELEMENT_COUNT_MAX = 60 * 24;

unsigned long storageLastChange = 0;
const long STORAGE_DELAY = 60000; // this is regestering as two minutes for some reason??????

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

    StorageManager(ModeManager* _modeManager){ 
        this->currentIndex = 0;
        this->humRange.max = 0;
        this->humRange.min = 100;
        this->tempRange.max = 0;
        this->tempRange.min = 100;
        this->storageFull = false;
        this->modeManager = _modeManager;
        this->initialised = false;
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

            if(validReadings()){
                this->readings[this->currentIndex] = this->lastRead;
                updateMinMax();
                this->currentIndex++;
            }

        }
    }

    bool validReadings(){

        // Checks for error state readings, mainly due to loose wiring 
        if (this->lastRead.temp > 500){
            return false;
        }

        if (this->lastRead.hum > 500){
            return false;
        }
        return true;
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

    void changeMode(int modeIndex){
        modeManager->setMode(modeIndex);
    }

    Mode* getCurrentMode(){
        return this->modeManager->getCurrentMode();
    }

    Mode* getCustMode(){
        return this->modeManager->custMode();
    }

    void setCustEnviro(MinMax newTempRange,MinMax newHumRange){
        modeManager->setNewEnviroValues(newTempRange,newHumRange);
    }

    void moistureLevels(int rangeChoice){

        MinMax soilRange;

        switch (rangeChoice){
            case 0:
                soilRange.min = 0;
                soilRange.max = 20;
                break;
            case 1:
                soilRange.min = 20;
                soilRange.max = 60;
                break;
            case 2:
                soilRange.min = 60;
                soilRange.max = 80;
                break;   
            default:    
                soilRange.min = 0;
                soilRange.max = 80;
                break;     
        }

        modeManager->setSoilRange(soilRange);
    }

    String getModeString(){

        String modeToReturn = "";

        switch(modeManager->getModeType()){
            case WINTER:  
                modeToReturn = "Winter";
                break;
            case GERMINATING:
                modeToReturn = "Germinating";
                break;
            case VEGATATIVE:
                modeToReturn = "Vegatative";
                break;
            case FRUITING:
                modeToReturn = "Fruiting";
                break;
            case NIGHT:
                modeToReturn = "Night";
                break;
            case CUSTOM:
                modeToReturn = "Custom";
                break;
        }
        return modeToReturn;
    }
    
private:
Readings readings[ELEMENT_COUNT_MAX];
Readings lastRead;
MinMax tempRange;
MinMax humRange;
uint8_t currentIndex;
boolean storageFull;
boolean initialised;
ModeManager* modeManager;

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
