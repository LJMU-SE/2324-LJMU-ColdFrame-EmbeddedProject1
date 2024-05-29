#ifndef SCREEN_H
#define SCREEN_H
#include <DisplayScreen.h>

const int NUM_MENU_OPTIONS = 2 + 1;
const int NUM_MODE_OPTIONS = 5 + 1; // +1 is for Go Back Option

struct TextElement
{
    char *label;
    char *value;
    int colour;
};

// Parent class
class Screen
{
public:
    Screen() {}

    virtual void paint(Adafruit_ST7735 *tftDisplay) = 0;
    virtual void updateValues(Readings latest, StorageManager *storageManager) = 0;
    virtual bool dirtyScreen(Readings latest) = 0;
    virtual int selectOption() = 0;
    

    virtual void setScrollStatus(bool action){
        scrollDue = action;
    }

    virtual void reset(Adafruit_ST7735 *tftDisplay){
       updatedRequired = true;
       this->paint(tftDisplay);
    }

protected:
    bool updatedRequired;
    bool scrollDue = false;
};

class MainMenu : public Screen
{

public:
    MainMenu() : Screen() {}

    // seperate storing attributes into different fcuntion
    // class should store readings then decide if the screen is "dirty" and needs to be repainted

    virtual void updateValues(Readings latest, StorageManager *storageManager)
    {

        this->tempRange = storageManager->getMinMaxTemp();
        this->humRange = storageManager->getMinMaxHum();

        this->updatedRequired = dirtyScreen(latest);
        this->currentValues = latest;
    }

    virtual int selectOption(){}

    virtual bool dirtyScreen(Readings latest)
    {
        if (latest.temp != this->currentValues.temp)
            return true;
        if (latest.hum != this->currentValues.hum)
            return true;
        if (latest.moist != this->currentValues.moist)
            return true;
        if (latest.soilState != this->currentValues.soilState)
            return true;
        if (latest.envState != this->currentValues.envState)
            return true;
        return false;
    }

    virtual void paint(Adafruit_ST7735 *tftDisplay) override
    {

        if (!updatedRequired)
        {
            return;
        }

        Serial.println("Repainting screen");
        tftDisplay->setFont();
        tftDisplay->fillScreen(ST77XX_BLACK);

        tftDisplay->setTextSize(1);
        tftDisplay->setCursor(0, 0);

        tftDisplay->setTextColor(ST77XX_ORANGE);
        tftDisplay->print("* ");
        tftDisplay->setTextColor(ST77XX_WHITE);
        tftDisplay->print("Temp: ");
        tftDisplay->print(currentValues.temp);
        tftDisplay->println("C");
        tftDisplay->println("------------------------------------------");
        tftDisplay->setTextColor(ST77XX_RED);
        tftDisplay->print("* ");
        tftDisplay->setTextColor(ST77XX_WHITE);
        tftDisplay->print("Humidity: ");
        tftDisplay->print(currentValues.hum);
        tftDisplay->println("%");
        tftDisplay->println("------------------------------------------");
        tftDisplay->setTextColor(ST77XX_BLUE);
        tftDisplay->print("* ");
        tftDisplay->setTextColor(ST77XX_WHITE);
        tftDisplay->print("Soil Moisture: ");
        tftDisplay->print(currentValues.moist);
        tftDisplay->println("%");
        tftDisplay->println("------------------------------------------");

        tftDisplay->print("Soil state: ");
        tftDisplay->setTextColor(currentValues.soilState == "Too Low" || currentValues.soilState == "Too High" ? ST7735_RED : ST7735_GREEN);
        tftDisplay->println(currentValues.soilState);
        tftDisplay->setTextColor(ST7735_WHITE);
        tftDisplay->print("Env state: ");
        tftDisplay->setTextColor(currentValues.envState == "Too Low" || currentValues.envState == "Too High" ? ST7735_RED : ST7735_GREEN);
        tftDisplay->println(currentValues.envState);
        tftDisplay->setTextColor(ST7735_WHITE);
        tftDisplay->println("---------------------");
        tftDisplay->setTextColor(ST7735_GREEN);
        tftDisplay->println("24hr Ranges");
        tftDisplay->setTextColor(ST7735_WHITE);
        tftDisplay->println("---------------------");
        tftDisplay->printf("Temp Range: %dC-%dC\n", tempRange.min, tempRange.max);
        tftDisplay->printf("Hum Range: %d-%d%%", humRange.min, humRange.max);

        this->updatedRequired = false;
    }

protected:
    Readings currentValues;
    MinMax tempRange;
    MinMax humRange;
};

class EditMenu : public Screen
{

public:
    EditMenu() : Screen()
    {

        this->menuOptions[0] = "1. Select Mode\n";
        this->menuOptions[1] = "2. Custom Settings\n";
        this->menuOptions[2] = "Go Back\n";

    }

    // needs to be responsible for whether it needs to repaint
    // update then paint

    virtual void updateValues(Readings latest, StorageManager *storageManager)
    {   
          
            if (scrollDue){
                this->updatedRequired = true;
                if(currentFocusIndex + 1 >= NUM_MENU_OPTIONS){
                    currentFocusIndex = 0;
                }
                else{
                    currentFocusIndex++;
                    
                }
                scrollDue = false;
            }
    }

    virtual bool dirtyScreen(Readings latest){
        return true;
    }

    virtual int selectOption(){
        return this->currentFocusIndex;
    }

    virtual void paint(Adafruit_ST7735 *tftDisplay) override
    {

        if (!updatedRequired)
            return;

        tftDisplay->setFont();
        tftDisplay->fillScreen(ST77XX_BLACK);

        tftDisplay->setTextSize(2);
        tftDisplay->setCursor(0, 0);

        tftDisplay->setTextColor(ST77XX_ORANGE);
        tftDisplay->println("Edit Menu");
        tftDisplay->println("----------");
        tftDisplay->setTextSize(1);
        tftDisplay->setTextColor(ST77XX_WHITE);

        for (int i = 0; i < NUM_MENU_OPTIONS; i++)
        {
            tftDisplay->setTextColor(decideColour(i));
            tftDisplay->println(menuOptions[i]);
        }
        this->updatedRequired = false;
    }

protected:
    int currentFocusIndex = 0;
    int prevFocusIndex = 0;
    String menuOptions[NUM_MENU_OPTIONS]; // +1 is for go back option


    int decideColour(int index)
    {
        if (index == currentFocusIndex)
        {
            return ST7735_GREEN;
        }
        else
        {
            return ST7735_WHITE;
        }
    }
};

class ModeList : public Screen
{

public:
    ModeList() : Screen()
    {

        this->menuOptions[0] = "1. Winter\n";
        this->menuOptions[1] = "2. Germinating\n";
        this->menuOptions[2] = "3. Vegatative\n";
        this->menuOptions[3] = "4. Fruiting\n";
        this->menuOptions[4] = "5. Night\n";
        this->menuOptions[5] = "Go Back\n";

    }

    // needs to be responsible for whether it needs to repaint
    // update then paint

    virtual void updateValues(Readings latest, StorageManager *storageManager)
    {   
          
            if (scrollDue){
                this->updatedRequired = true;
                if(currentFocusIndex + 1 >= NUM_MODE_OPTIONS){
                    currentFocusIndex = 0;
                }
                else{
                    currentFocusIndex++;
                    
                }
                scrollDue = false;
            }
    }

    virtual bool dirtyScreen(Readings latest){
        return true;
    }

    virtual int selectOption(){







        return this->currentFocusIndex;


    }

    virtual void paint(Adafruit_ST7735 *tftDisplay) override
    {

        if (!updatedRequired)
            return;

        tftDisplay->setFont();
        tftDisplay->fillScreen(ST77XX_BLACK);

        tftDisplay->setTextSize(1.5);
        tftDisplay->setCursor(0, 0);

        tftDisplay->setTextColor(ST77XX_ORANGE);
        tftDisplay->println("Select Mode");
        tftDisplay->println("----------");
        tftDisplay->setTextSize(1);
        tftDisplay->setTextColor(ST77XX_WHITE);

        for (int i = 0; i < NUM_MODE_OPTIONS; i++)
        {
            tftDisplay->setTextColor(decideColour(i));
            tftDisplay->println(menuOptions[i]);
        }
        this->updatedRequired = false;
    }

protected:
    int currentFocusIndex = 0;
    int prevFocusIndex = 0;
    String menuOptions[NUM_MODE_OPTIONS + 1]; // +1 is for the Go back option


    int decideColour(int index)
    {
        if (index == currentFocusIndex)
        {
            return ST7735_GREEN;
        }
        else
        {
            return ST7735_WHITE;
        }
    }
};

#endif