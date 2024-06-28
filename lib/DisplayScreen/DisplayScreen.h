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
    MODE_LIST,
    CUSTOMISE
};

const long DEBUG_DELAY = 5000;
unsigned long debugLastChange = 0;

class DisplayScreen
{
public:
    DisplayScreen(NumericalControl *_numericalControl)
    {

        // Initialise the display
        this->tftDisplay.initR(INITR_144GREENTAB);
        this->canvas = new GFXcanvas16(tftDisplay.width(), tftDisplay.height());
        Serial.println("Display Active");
        tftDisplay.fillScreen(ST77XX_BLACK);

        // Define screens
        this->showing = MAIN_MENU;
        this->screens[0] = new MainMenu();
        this->screens[1] = new EditMenu();
        this->screens[2] = new ModeList();
        this->screens[3] = new Customise();

        // Assign controller
        this->numericalControl = _numericalControl;
    }

    void processButtonPress(StorageManager* storageManager){ // Handles larger actions and menu changes

        int menuIndex;
        menuIndex = this->screens[this->showing]->selectOption();

        switch (this->showing){
            case MAIN_MENU:
                changeTo(EDIT_MENU);
                break;

            case EDIT_MENU:

                if (menuIndex == -1){
                    changeTo(MAIN_MENU);
                    
                }
                else if (menuIndex == 1){
                    changeTo(CUSTOMISE);
                }
                else{
                    changeTo(MODE_LIST);
                }
                break; 

            case MODE_LIST:
                // Returns -1 if go back option selected
                if (menuIndex >= 0){
                    storageManager->changeMode(menuIndex);
                    changeTo(MAIN_MENU);
                }
                else {
                    changeTo(EDIT_MENU);
                }  
                break;    

            case CUSTOMISE:
                if (menuIndex == -1){
                    changeTo(EDIT_MENU);
                }
                break;

        }
        this->screens[this->showing]->reset(&tftDisplay,canvas);

    }

    void changeTo(DisplayState screen){


        switch(screen){

            case MODE_LIST:
                this->showing = MODE_LIST;
                Serial.println("\n-----Changing to Mode List-----\n");
                break;
            case MAIN_MENU:
                this->showing = MAIN_MENU;
                Serial.println("\n-----Changing to Main Menu-----\n");
                break;
            case EDIT_MENU:
                this->showing = EDIT_MENU;
                Serial.println("\n-----Changing to Edit Menu------\n");
                break;
            case CUSTOMISE:
                this->showing = CUSTOMISE;
                Serial.println("\n-----Changing to Customise Menu------\n");    
        }
    }

    void update(Readings readings, StorageManager *storageManager)
    {

        // Listen for controller input
        this->numericalControl->update();
        bool isPressed = this->numericalControl->wasPressed();

        // Compare current button state to previous
        if (wasPressed != isPressed)
        {
            Serial.println("press changed");
            wasPressed = isPressed;
            if (isPressed) 
                processButtonPress(storageManager); // Handles system navigation and change presses
            this->screens[this->showing]->setButtonStatus(isPressed); // Handles changes unique to screen object
            

        }

        // Check if scroll due
        bool shouldScroll = this->numericalControl->scroll();
        this->screens[this->showing]->setScrollStatus(shouldScroll);

        int direction = this->numericalControl->getDirection();
        this->screens[this->showing]->setDirection(direction);


        // Update screen values and repaint if neccesary
        this->screens[this->showing]->updateValues(readings, storageManager);
        this->screens[this->showing]->paint(&tftDisplay,canvas);
        this->updateTerminal(readings,storageManager);
    }

    void updateTerminal(Readings latest,StorageManager* storageManager)
    {

        unsigned long currentMillis = millis();
        

        if (currentMillis - debugLastChange >= DEBUG_DELAY)
        {
            debugLastChange = currentMillis;
            Serial.printf("-------------------Current Mode: %s---------------------\n",storageManager->getModeString());
            Serial.printf("|| Temperature %d° || Humidity %d%% || Environment State: %s \n", latest.temp, latest.hum, latest.envState);
            Serial.printf("-------------------------------------------------------------\n");
            Serial.printf("|| Soil Moisture: %d%% || Soil Moisture State: %s\n", latest.moist, latest.soilState);
            Serial.printf("-------------------------------------------------------------\n");
            Serial.println("\n");
        }
    }

private:
    Adafruit_ST7735 tftDisplay = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
    GFXcanvas16* canvas;
    DisplayState showing;

    // Array of pointers to abstract objects
    Screen *screens[4];
    bool wasPressed = false;
    NumericalControl *numericalControl;

};
#endif