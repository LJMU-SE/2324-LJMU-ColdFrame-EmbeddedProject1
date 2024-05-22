#ifndef DISPLAYSCREEN_H
#define DISPLAYSCREEN_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Arduino.h>
#include <SPI.h>
#include <Screen.h>

#define TFT_CS 5
#define TFT_RST 17  
#define TFT_DC   14 

enum DisplayState{
    MAIN_MENU,
    EDIT_MENU
};

const long DEBUG_DELAY = 5000;
unsigned long debugLastChange = 0;

class DisplayScreen{
public:

    DisplayScreen() {

        this->tftDisplay.initR(INITR_144GREENTAB); 
        Serial.println("Display Active");
        tftDisplay.fillScreen(ST77XX_BLACK);
        this->showing = MAIN_MENU; 

        this->screens[0] = new MainMenu();
        this->screens[1] = new EditMenu(); 

    }


    void update(Readings readings,StorageManager* storageManager){


        switch(this->showing){

            case MAIN_MENU:
                this->activeScreen = 0;
                break;    
            case EDIT_MENU:
                this->activeScreen = 1;
                break;

        }

        this->screens[activeScreen]->paint(readings,storageManager,&tftDisplay);
        this->updateTerminal(readings);

    }

    void updateTerminal(Readings latest){

        unsigned long currentMillis = millis();

        if (currentMillis - debugLastChange >= DEBUG_DELAY)
        {
            debugLastChange = currentMillis;
            Serial.printf("Debug String \n");
            Serial.printf("-------------------------------------------------------------\n");
            Serial.printf("|| Temperature %d° || Humidity %d%% || Environment State: %s \n", latest.temp, latest.hum, latest.envState);
            Serial.printf("-------------------------------------------------------------\n");
            Serial.printf("|| Soil Moisture: %d%% || Soil Moisture State: %s\n", latest.moist, latest.soilState);
            Serial.printf("-------------------------------------------------------------\n");
            Serial.println("\n"); 
        }
    }

    
private:

Adafruit_ST7735 tftDisplay = Adafruit_ST7735(TFT_CS, TFT_DC,TFT_RST);
DisplayState showing;

// Array of pointers to abstract objects 
Screen* screens[2];
u_int8_t activeScreen;


// array of pointers to abstracts

};
#endif 