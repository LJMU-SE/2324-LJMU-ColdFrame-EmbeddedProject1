#ifndef DISPLAYSCREEN_H
#define DISPLAYSCREEN_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Arduino.h>
#include <SPI.h>
#include <Screen.h>
#include <NumericalControl.h>
#include <ModeManager.h>

#define TFT_CS 5
#define TFT_RST 17
#define TFT_DC 14

enum DisplayState
{
    MAIN_MENU,
    EDIT_MENU,
    MODE_LIST
};

const long DEBUG_DELAY = 5000;
unsigned long debugLastChange = 0;

class DisplayScreen
{
public:
    DisplayScreen(NumericalControl *_numericalControl,ModeManager* _modeManager)
    {

        // Initialise the display
        this->tftDisplay.initR(INITR_144GREENTAB);
        Serial.println("Display Active");
        tftDisplay.fillScreen(ST77XX_BLACK);

        // Define menus
        this->showing = MAIN_MENU;
        this->screens[0] = new MainMenu();
        this->screens[1] = new EditMenu();
        this->screens[2] = new ModeList();


        // Assign controller
        this->numericalControl = _numericalControl;
        this->modeManager = _modeManager;
    }

    void processButtonPress(){


        switch (this->showing){
            case MAIN_MENU:
                this->showing = EDIT_MENU;
                Serial.println("\n-----Changing to Edit Menu------\n");
                break;

            case EDIT_MENU:
                this->showing = MODE_LIST;
                Serial.println("\n-----Changing to Mode List-----\n");
                break; 

            case MODE_LIST:
                int menuIndex = this->screens[this->showing]->selectOption();  
                modeManager->changeMode(menuIndex);
                this->showing = MAIN_MENU;
                Serial.println("\n-----Changing to Main Menu-----\n");
                break;         
        }
        this->screens[this->showing]->reset(&tftDisplay);

    }

    void update(Readings readings, StorageManager *storageManager)
    {

        // Listen for controller input
        this->numericalControl->update();
        bool isPressed = this->numericalControl->wasPressed();

        // Compare current state to previous
        if (wasPressed != isPressed)
        {
            wasPressed = isPressed;
            if (isPressed)
                processButtonPress();
        }

        // Janky but will do for now :) awful awful awful DO NOT READ
        bool shouldScroll = this->numericalControl->scroll();
        this->screens[1]->setScrollStatus(shouldScroll);
        this->screens[2]->setScrollStatus(shouldScroll);

        // Update screen values and repaint if neccesary
        this->screens[this->showing]->updateValues(readings, storageManager);
        this->screens[this->showing]->paint(&tftDisplay);
        this->updateTerminal(readings);
    }

    void updateTerminal(Readings latest)
    {

        unsigned long currentMillis = millis();

        if (currentMillis - debugLastChange >= DEBUG_DELAY)
        {
            debugLastChange = currentMillis;
            Serial.printf("-------------------Current Mode: %d---------------------\n", this->modeManager->getCurrentMode()->getModeType());
            Serial.printf("|| Temperature %d° || Humidity %d%% || Environment State: %s \n", latest.temp, latest.hum, latest.envState);
            Serial.printf("-------------------------------------------------------------\n");
            Serial.printf("|| Soil Moisture: %d%% || Soil Moisture State: %s\n", latest.moist, latest.soilState);
            Serial.printf("-------------------------------------------------------------\n");
            Serial.println("\n");
        }
    }

private:
    Adafruit_ST7735 tftDisplay = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
    DisplayState showing;

    // Array of pointers to abstract objects
    Screen *screens[3];
    bool wasPressed = false;
    NumericalControl *numericalControl;
    ModeManager* modeManager;
};
#endif