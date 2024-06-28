#ifndef SCREEN_H
#define SCREEN_H
#include <DisplayScreen.h>

// Parent class
class Screen
{
public:
    Screen() {}

    virtual void paint(Adafruit_ST7735 *tftDisplay, GFXcanvas16 *canvas) = 0;
    virtual void updateValues(Readings latest, StorageManager *storageManager) = 0;

    virtual void setScrollStatus(bool action)
    {
        scrollDue = action;
    }

    virtual void setButtonStatus(bool action)
    {
        buttonPress = action;
    }

    virtual void reset(Adafruit_ST7735 *tftDisplay, GFXcanvas16 *canvas)
    {
        updatedRequired = true;
        this->paint(tftDisplay, canvas);
    }

    virtual void setDirection(int dir)
    {
        this->direction = dir;
    }

    virtual int selectOption()
    {

        if (this->currentFocusIndex >= this->numOptions - 1)
        {
            return -1;
        }
        return this->currentFocusIndex;
    }

protected:
    bool updatedRequired;
    bool scrollDue = false;
    bool buttonPress = false;
    int direction;
    int numOptions;
    int currentFocusIndex = 0;
};

class MainMenu : public Screen
{

public:
    MainMenu() : Screen() {}

    virtual void updateValues(Readings latest, StorageManager *storageManager)
    {
        this->tempRange = storageManager->getMinMaxTemp();
        this->humRange = storageManager->getMinMaxHum();

        this->updatedRequired = dirtyScreen(latest);
        this->currentValues = latest;
        this->currentMode = storageManager->getModeString();
    }

    // virtual int selectOption() override{
    //     return 0; // Unused on this screen for now
    // }

    bool dirtyScreen(Readings latest)
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

    virtual void paint(Adafruit_ST7735 *tftDisplay, GFXcanvas16 *canvas) override
    {

        if (!updatedRequired)
        {
            return;
        }

        canvas->setFont();
        canvas->fillScreen(ST77XX_BLACK);

        canvas->setTextSize(1);
        canvas->setCursor(0, 0);

        canvas->setTextColor(ST77XX_WHITE);
        canvas->print("Mode: ");
        canvas->println(this->currentMode);
        canvas->println("---------------------");

        canvas->setTextColor(ST77XX_ORANGE);
        canvas->println();
        canvas->print("* ");
        canvas->setTextColor(ST77XX_WHITE);
        canvas->print("Temp: ");
        canvas->setTextColor(currentValues.envState == "Too Low" || currentValues.envState == "Too High" ? ST7735_RED : ST7735_GREEN);
        canvas->print(currentValues.temp);
        canvas->println("C");
        canvas->println();
        canvas->setTextColor(ST77XX_RED);
        canvas->print("* ");
        canvas->setTextColor(ST77XX_WHITE);
        canvas->print("Humidity: ");
        canvas->setTextColor(currentValues.envState == "Too Low" || currentValues.envState == "Too High" ? ST7735_RED : ST7735_GREEN);
        canvas->print(currentValues.hum);
        canvas->println("%");
        canvas->println();
        canvas->setTextColor(ST77XX_BLUE);
        canvas->print("* ");
        canvas->setTextColor(ST77XX_WHITE);
        canvas->print("Soil Moisture: ");
        canvas->setTextColor(currentValues.soilState == "Too Low" || currentValues.soilState == "Too High" ? ST7735_RED : ST7735_GREEN);
        canvas->print(currentValues.moist);
        canvas->println("%");
        canvas->println();

        canvas->setTextColor(ST7735_WHITE);
        canvas->println("---------------------");
        canvas->setTextColor(ST7735_GREEN);
        canvas->println("24hr Ranges");
        canvas->setTextColor(ST7735_WHITE);
        canvas->println("---------------------");
        canvas->printf("Temp: %dC - %dC\n", tempRange.min, tempRange.max);
        canvas->println();
        canvas->printf("Hum: %d - %d%%", humRange.min, humRange.max);

        tftDisplay->drawRGBBitmap(0, 0, canvas->getBuffer(), canvas->width(), canvas->height());

        this->updatedRequired = false;
    }

protected:
    Readings currentValues;
    MinMax tempRange;
    MinMax humRange;
    String currentMode;
};

class EditMenu : public Screen
{

public:
    EditMenu() : Screen()
    {

        this->menuOptions[0] = "1. Select Mode\n";
        this->menuOptions[1] = "2. Custom Settings\n";
        this->menuOptions[2] = "Go Back\n";

        this->numOptions = 3;
    }

    virtual void updateValues(Readings latest, StorageManager *storageManager)
    {

        if (scrollDue)
        {
            this->updatedRequired = true;
            currentFocusIndex += this->direction;

            if (currentFocusIndex >= this->numOptions || currentFocusIndex < 0)
            {
                currentFocusIndex = 0;
            }

            scrollDue = false;
        }
    }

    virtual void paint(Adafruit_ST7735 *tftDisplay, GFXcanvas16 *canvas) override
    {

        if (!updatedRequired)
            return;

        canvas->setFont();
        canvas->fillScreen(ST77XX_BLACK);

        canvas->setTextSize(2);
        canvas->setCursor(0, 0);

        canvas->setTextColor(ST77XX_ORANGE);
        canvas->println("Edit Menu");
        canvas->println("----------");
        canvas->setTextSize(1);
        canvas->setTextColor(ST77XX_WHITE);

        for (int i = 0; i < this->numOptions; i++)
        {
            canvas->setTextColor(decideColour(i));
            canvas->println(menuOptions[i]);
        }

        tftDisplay->drawRGBBitmap(0, 0, canvas->getBuffer(), canvas->width(), canvas->height());
        this->updatedRequired = false;
    }

protected:
    String menuOptions[3];

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
        this->menuOptions[5] = "6. Custom\n";
        this->menuOptions[6] = "Go Back\n";

        this->numOptions = 7;
    }

    virtual void updateValues(Readings latest, StorageManager *storageManager)
    {

        if (scrollDue)
        {

            this->updatedRequired = true;
            currentFocusIndex += this->direction;

            if (currentFocusIndex >= this->numOptions || currentFocusIndex < 0)
            {
                currentFocusIndex = 0;
            }

            scrollDue = false;
        }
    }

    virtual void paint(Adafruit_ST7735 *tftDisplay, GFXcanvas16 *canvas) override
    {

        if (!updatedRequired)
            return;

        canvas->setFont();
        canvas->fillScreen(ST77XX_BLACK);

        canvas->setTextSize(1.5);
        canvas->setCursor(0, 0);

        canvas->setTextColor(ST77XX_ORANGE);
        canvas->println("Select Mode");
        canvas->println("----------");
        canvas->setTextSize(1);
        canvas->setTextColor(ST77XX_WHITE);

        for (int i = 0; i < this->numOptions; i++)
        {
            canvas->setTextColor(decideColour(i));
            canvas->println(menuOptions[i]);
        }

        tftDisplay->drawRGBBitmap(0, 0, canvas->getBuffer(), canvas->width(), canvas->height());
        this->updatedRequired = false;
    }

protected:
    String menuOptions[7];

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

class Customise : public Screen
{

public:
    Customise() : Screen()
    {
        this->tempRange.min = -1;
        this->tempRange.max = -1;
        this->humRange.min = -1;
        this->humRange.max = -1;
        this->soilRange.min = -1;
        this->soilRange.max = -1;
        this->menuOptions[7] = "\nGo Back\n";
        this->numOptions = 8;
    }

    bool dirtyScreen(MinMax temp, MinMax hum, MinMax soil)
    {

        if (temp.min != this->tempRange.min || temp.max != this->tempRange.max)
            return true;
        if (hum.min != this->humRange.min || hum.max != this->humRange.max)
            return true;
        if (soil.min != this->soilRange.min || soil.max != this->soilRange.max)
            return true;
        return false;
    }

    bool oldButtonPress = false;

    virtual void updateValues(Readings latest, StorageManager *storageManager)
    {
        Mode *custMode = storageManager->getCustMode();

        MinMax newTempRange = custMode->getTempRange();
        MinMax newHumRange = custMode->getHumidityRange();
        MinMax newSoilRange = custMode->getSoilRange();

        if (buttonPress != oldButtonPress)
        {
            // this is messy AF - needs thought
            oldButtonPress = buttonPress;

            if (buttonPress)
            {
                if (!valueSelected)
                {

                    this->selectedValueIndex = currentFocusIndex;
                    // Only selects value if it is a temp or humidity range
                    switch (selectedValueIndex)
                    {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                        this->valueSelected = true;
                        break;
                    case 4:
                        // set low soil range
                        storageManager->moistureLevels(0);
                        break;
                    case 5:
                        // set med soil range
                        storageManager->moistureLevels(1);
                        break;
                    case 6:
                        // set high soil range
                        storageManager->moistureLevels(2);
                        break;
                    }
                }
                else
                {
                    this->selectedValueIndex = -1;
                    this->valueSelected = false;
                }
                this->updatedRequired = true;
            }
        }

        if (scrollDue)
        {

            if (!valueSelected)
            { // Cycle options

                this->currentFocusIndex += direction;
                this->updatedRequired = true;

                if (this->currentFocusIndex >= this->numOptions)
                {
                    this->currentFocusIndex = 0;
                }

                scrollDue = false;
            }
            else
            {

                // Iterate value logic
                switch (selectedValueIndex)
                {
                case 0:
                    newTempRange.min += this->direction;
                    break;
                case 1:
                    newTempRange.max += this->direction;
                    break;
                case 2:
                    newHumRange.min += this->direction;
                    break;
                case 3:
                    newHumRange.max += this->direction;
                    break;
                }
                storageManager->setCustEnviro(newTempRange, newHumRange);
            }
        }

        if (dirtyScreen(newTempRange, newHumRange, newSoilRange))
        {
            updatedRequired = true;
            this->tempRange = newTempRange;
            this->humRange = newHumRange;
            this->soilRange = newSoilRange;
        }

        this->menuOptions[0] = String(tempRange.min);
        this->menuOptions[1] = String(tempRange.max);
        this->menuOptions[2] = String(humRange.min);
        this->menuOptions[3] = String(humRange.max);

        this->menuOptions[4] = "Low";
        this->menuOptions[5] = "Med";
        this->menuOptions[6] = "High\n";

        if (soilRange.max < 30)
        {
            this->menuOptions[4] = "*Low*";
        }
        else if (soilRange.max >= 30 && soilRange.max <= 60)
        {
            this->menuOptions[5] = "*Med*";
        }
        else if (soilRange.max > 60)
        {
            this->menuOptions[6] = "*High*\n";
        }
    }

    virtual void paint(Adafruit_ST7735 *tftDisplay, GFXcanvas16 *canvas) override
    {

        if (!updatedRequired)
            return;

        canvas->setFont();
        canvas->fillScreen(ST77XX_BLACK);
        canvas->setTextSize(1);
        canvas->setCursor(0, 0);

        canvas->setTextColor(ST77XX_ORANGE);
        canvas->println("Custom Mode");
        canvas->setTextColor(ST77XX_WHITE);

        canvas->println("--------------------");
        canvas->println("Temperature Range:");
        canvas->println("--------------------");

        canvas->setTextColor(decideColour(0));
        canvas->print(this->menuOptions[0]);
        canvas->setTextColor(ST77XX_WHITE);

        canvas->print(" - ");
        canvas->setTextColor(decideColour(1));
        canvas->print(this->menuOptions[1]);
        canvas->setTextColor(ST77XX_WHITE);
        canvas->println("  C");

        canvas->println("--------------------");

        canvas->println("Humidity Range:");
        canvas->println("--------------------");

        canvas->setTextColor(decideColour(2));
        canvas->print(this->menuOptions[2]);
        canvas->setTextColor(ST77XX_WHITE);
        canvas->print(" - ");
        canvas->setTextColor(decideColour(3));
        canvas->print(this->menuOptions[3]);
        canvas->setTextColor(ST77XX_WHITE);
        canvas->println("  %");

        canvas->setTextSize(1);

        canvas->println("--------------------");
        canvas->println("Moisture Range: ");
        canvas->println("--------------------");

        canvas->setTextColor(decideColour(4));
        canvas->print(this->menuOptions[4]);
        canvas->setTextColor(ST77XX_WHITE);
        canvas->print(" - ");
        canvas->setTextColor(decideColour(5));
        canvas->print(this->menuOptions[5]);
        canvas->setTextColor(ST77XX_WHITE);
        canvas->print(" - ");
        canvas->setTextColor(decideColour(6));
        canvas->print(this->menuOptions[6]);
        canvas->setTextColor(decideColour(7));
        canvas->print(this->menuOptions[7]);

        tftDisplay->drawRGBBitmap(0, 0, canvas->getBuffer(), canvas->width(), canvas->height());
        this->updatedRequired = false;
    }

protected:
    String menuOptions[8];
    MinMax tempRange;
    MinMax humRange;
    MinMax soilRange;
    bool valueSelected = false;
    int selectedValueIndex = -1;

    int decideColour(int index)
    {
        if (valueSelected)
        {
            if (index == selectedValueIndex)
            {
                return ST7735_ORANGE;
            }
            else
            {
                return ST7735_WHITE;
            }
        }

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