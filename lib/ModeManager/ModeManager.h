#ifndef MODEMANAGER_H
#define MODEMANAGER_H
#include <Modes.h>
#include<Arduino.h>


class ModeManager {
public:
    ModeManager(){
        
        // Initialise Modes and set default mode to Vegatative
        this->availableModes[0] = new Mode(ModeType::WINTER, 0, 200, 0, 200, 0, 20);
        this->availableModes[1] = new Mode(ModeType::GERMINATING, 18, 32, 70, 95, 0, 60);
        this->availableModes[2] = new Mode(ModeType::VEGATATIVE, 20, 25, 60, 70, 40, 80);
        this->availableModes[3] = new Mode(ModeType::FRUITING, -200, 28, 40, 50, 40, 80);
        this->availableModes[4] = new Mode(ModeType::NIGHT, 15, 30, 30, 80, 0, 20);

        this->currentMode = this->availableModes[2];
    }


    Mode* getCurrentMode(){
        return this->currentMode;
    }

    void changeMode(int index){
        Serial.println("------ Mode Changed --------");
        this->currentMode = this->availableModes[index];
    }



private:

Mode* availableModes[5];
Mode* currentMode;

};
#endif 