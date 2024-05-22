#ifndef SCREEN_H
#define SCREEN_H
#include<DisplayScreen.h>

const int NUM_MENU_OPTIONS = 2;

struct TextElement{
    char* label;
    char* value;
    int colour;
};

// Parent class
class Screen {
public:

    Screen(){}

    virtual void paint(Readings latest,StorageManager* storageManager,Adafruit_ST7735* tftDisplay) = 0;

protected:

};


class MainMenu : public Screen{

public:

    MainMenu() : Screen() {}

    // seperate storing attributes into different fcuntion 
    // class should store readings then decide if the screen is "dirty" and needs to be repainted

    virtual void paint(Readings latest,StorageManager* storageManager,Adafruit_ST7735* tftDisplay) override {

        if(!storageManager->levelChange(latest)){
            return;
        }

        MinMax tempRange = storageManager->getMinMaxTemp();
        MinMax humRange = storageManager->getMinMaxHum();
        
        tftDisplay->setFont();
        tftDisplay->fillScreen(ST77XX_BLACK);
        
        tftDisplay->setTextSize(1);
        tftDisplay->setCursor(0,0);
        
        tftDisplay->setTextColor(ST77XX_ORANGE);
        tftDisplay->print("* ");
        tftDisplay->setTextColor(ST77XX_WHITE);
        tftDisplay->print("Temp: ");
        tftDisplay->print(latest.temp);
        tftDisplay->println("C");
        tftDisplay->println("------------------------------------------");
        tftDisplay->setTextColor(ST77XX_RED);
        tftDisplay->print("* ");
        tftDisplay->setTextColor(ST77XX_WHITE);
        tftDisplay->print("Humidity: ");
        tftDisplay->print(latest.hum);
        tftDisplay->println("%");
        tftDisplay->println("------------------------------------------");
        tftDisplay->setTextColor(ST77XX_BLUE);
        tftDisplay->print("* ");
        tftDisplay->setTextColor(ST77XX_WHITE);
        tftDisplay->print("Soil Moisture: ");
        tftDisplay->print(latest.moist);
        tftDisplay->println("%");
        tftDisplay->println("------------------------------------------");
        
        tftDisplay->print("Soil state: ");
        tftDisplay->setTextColor(latest.soilState == "Too Low" || latest.soilState == "Too High" ? ST7735_RED : ST7735_GREEN);
        tftDisplay->println(latest.soilState);
        tftDisplay->setTextColor(ST7735_WHITE);
        tftDisplay->print("Env state: ");
        tftDisplay->setTextColor(latest.envState == "Too Low" || latest.envState == "Too High" ? ST7735_RED : ST7735_GREEN);
        tftDisplay->println(latest.envState);
        tftDisplay->setTextColor(ST7735_WHITE);
        tftDisplay->println("---------------------");
        tftDisplay->setTextColor(ST7735_GREEN);
        tftDisplay->println("24hr Ranges");
        tftDisplay->setTextColor(ST7735_WHITE);
        tftDisplay->println("---------------------");
        tftDisplay->printf("Temp Range: %dC-%dC\n",tempRange.min,tempRange.max);
        tftDisplay->printf("Hum Range: %d-%d%%",humRange.min,humRange.max);
    }
};


class EditMenu : public Screen{

public:

    EditMenu() : Screen() {

        this->menuOptions[0] = "1. Select Mode\n";
        this->menuOptions[1] = "2. Custom Settings";
    }

    // needs to be responsible for whether it needs to repaint 
    // update then paint 

    virtual void paint(Readings latest,StorageManager* storageManager,Adafruit_ST7735* tftDisplay) override {


        if(!storageManager->levelChange(latest)){
            return;
        }

        MinMax tempRange = storageManager->getMinMaxTemp();
        MinMax humRange = storageManager->getMinMaxHum();
        
        tftDisplay->setFont();
        tftDisplay->fillScreen(ST77XX_BLACK);
        
        tftDisplay->setTextSize(2);
        tftDisplay->setCursor(0,0);
        
        tftDisplay->setTextColor(ST77XX_ORANGE);
        tftDisplay->println("Edit Menu");
        tftDisplay->println("----------");
        tftDisplay->setTextSize(1);
        tftDisplay->setTextColor(ST77XX_WHITE);
        
        for (int i = 0; i < NUM_MENU_OPTIONS; i++){
            tftDisplay->setTextColor(decideColour(i));
            tftDisplay->println(menuOptions[i]);
        }

    }

protected:

int currentFocusIndex = 0;
int prevFocusIndex = 0;
String menuOptions[NUM_MENU_OPTIONS];

int decideColour(int index){

    if (index == currentFocusIndex){
        return ST7735_GREEN;
    }
    else{
        return ST7735_WHITE;
    }
}
};





#endif