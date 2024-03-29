#ifndef DISPLAYSCREEN_H
#define DISPLAYSCREEN_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Arduino.h>

#define TFT_CS  27
#define TFT_RST  14 
#define TFT_DC   17
#define TFT_MOSI 23  
#define TFT_SCLK 15  

enum DisplayState{
    MAIN_DISPLAY,
    EDIT_MENU
};

const long DEBUG_DELAY = 5000;
unsigned long debugLastChange = 0;

class DisplayScreen{
public:

    DisplayScreen() {

        this->screen.initR(INITR_144GREENTAB); 
        screen.fillScreen(ST77XX_BLACK);
        screen.setTextColor(ST77XX_MAGENTA);
        screen.setTextSize(2);
        screen.setCursor(0,0);
        screen.print("Hello, ");
        screen.println("Hiya!");

        Serial.println("started screen");
        screen.fillScreen(ST77XX_BLACK);
        this->showing = MAIN_DISPLAY;
    }

    void toggle(){
        this->showing = this->showing = MAIN_DISPLAY ? EDIT_MENU : MAIN_DISPLAY;
    }

    void update(Readings readings,StorageManager* storageManager){

        this->updateTerminal(readings);

        if (this->showing = MAIN_DISPLAY){
            if (storageManager->levelChange(readings))
                this->updateTerminal(readings);
                this->updateDisplay(readings);
        }
    }

    void updateTerminal(Readings latest){

        unsigned long currentMillis = millis();

        if (currentMillis - debugLastChange >= DEBUG_DELAY)
        {
            debugLastChange = currentMillis;
            Serial.printf("Debug String \n");
            Serial.printf("-------------------------------------------------------------\n");
            Serial.printf("|| Temperature %d° || Humidity %d%% || Environment State: %s \n", readings.temp, readings.hum, readings.envState);
            Serial.printf("-------------------------------------------------------------\n");
            Serial.printf("|| Soil Moisture: %d%% || Soil Moisture State: %s\n", readings.moist, readings.soilState);
            Serial.printf("-------------------------------------------------------------\n");
            Serial.println("\n"); 
        }
    }

    void updateDisplay(Readings latest){
                
        screen.setFont();
        screen.fillScreen(ST77XX_BLACK);
        screen.setTextColor(ST77XX_WHITE);
        screen.setTextSize(1);
        screen.setCursor(0,0);

        screen.print("Temp: ");
        screen.print(latest.temp);
        screen.println(" C");
        screen.println("------------------------------------------");
        screen.print("Humidity: ");
        screen.print(latest.hum);
        screen.println(" %");
        screen.println("------------------------------------------");
        screen.print("Soil Moisture: ");
        screen.print(latest.moist);
        screen.println(" %");
        screen.println("------------------------------------------");
        
        screen.print("Soil state: ");
        screen.setTextColor(latest.soilState == "Too Low" || latest.soilState == "Too High" ? ST7735_RED : ST7735_GREEN);
        screen.println(latest.soilState);
        screen.setTextColor(ST7735_WHITE);
        screen.print("Env state: ");
        screen.setTextColor(latest.envState == "Too Low" || latest.envState == "Too High" ? ST7735_RED : ST7735_GREEN);
        screen.println(latest.envState);
        screen.setTextColor(ST7735_WHITE);
        screen.println("---------------------");
    }

private:

Adafruit_ST7735 screen = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
DisplayState showing;


};
#endif 