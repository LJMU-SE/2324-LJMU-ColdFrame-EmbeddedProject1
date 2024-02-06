#include <Arduino.h>
#include <DHTesp.h>
#include <RGBLed.h>

// Stores operating modes
enum Mode
{
    WINTER,
    GERMINATING,
    VEGATATIVE,
    FRUITING,
    NIGHT
};

// Stores state indicators
enum State
{
    TOO_HIGH,
    LEVELS_OK,
    TOO_LOW
};

// Stores sensor ranges
struct Boundaries
{
    int min;
    int max;
};

// Soil sensor readings
const int NO_WATER_READING = 2640;
const int SUBMERGED_READING = 1250;

const long BLINK_RATE = 350;

// Allows for DHT sensor reading
DHTesp DHT;

// Parent class for components
class Component
{
public:
    // Constructor
    Component(int pin)
    {
        this->pin = pin;
    }

    // Updates value and changes state if needed
    virtual void update() = 0;

    // Sets mode and changes boundaries accordingly
    virtual void setMode(Mode newMode) = 0;

    // Returns current state
    virtual State getStatus()
    {
        return this->status;
    }

protected:
    int pin;
    Mode operatingMode;
    State status;
};

// Child class for Soil Sensor
class SoilSensor : public Component
{
public:
    // Constructor
    SoilSensor(int pin) : Component(pin) {}

    // Updates value and changes state if neccesary
    virtual void update() override
    {
        
        // Read current value and map the value between no moisture and submerged in water
        this->moistureLevel = analogRead(this->pin);
        this->moisturePercent = map(this->moistureLevel, NO_WATER_READING, SUBMERGED_READING, 0, 100);

        if (this->moisturePercent > moistureLimit.max)
        {
            this->status = TOO_HIGH;
        }
        else if (this->moisturePercent < moistureLimit.min)
        {
            this->status = TOO_LOW;
        }
        else
        {
            this->status = LEVELS_OK;
        }
    }

    // Sets mode and changes boundaries accordingly
    virtual void setMode(Mode newMode) override
    {
        this->operatingMode = newMode;

        switch (this->operatingMode)
        {
        case WINTER:
            moistureLimit.min = 0;
            moistureLimit.max = 20;
            break;
        case GERMINATING:
            moistureLimit.min = 0;
            moistureLimit.max = 60;
            break;
        case VEGATATIVE:
        case FRUITING:
            moistureLimit.min = 40;
            moistureLimit.max = 80;
            break;
        case NIGHT:
            moistureLimit.min = 0;
            moistureLimit.max = 20;
            break;
        }
    }

    int getMoisturePercent()
    {
        return this->moisturePercent;
    }

protected:
    Boundaries moistureLimit;
    int moistureLevel;
    int moisturePercent;
};

// Child class for DHT sensor
class Enviroment : public Component
{
public:
    // Constructor
    Enviroment(int pin) : Component(pin)
    {
        this->pin = pin;
        DHT.setup(pin, DHTesp::DHT11);
    }

    // Updates temp and humidity and checks if state needs to change
    virtual void update() override
    {

        this->currentTemp = int(DHT.getTemperature());
        this->currentHumidity = int(DHT.getHumidity());

        if (currentTemp > tempRange.max || currentHumidity > humidityRange.max)
        {
            this->status = TOO_HIGH;
        }
        else if (currentTemp < tempRange.min || currentHumidity < humidityRange.min)
        {
            this->status = TOO_LOW;
        }
        else
        {
            this->status = LEVELS_OK;
        }
    }

    // Sets mode and changes boundaries accordingly
    virtual void setMode(Mode newMode) override
    {
        this->operatingMode = newMode;

        switch (this->operatingMode)
        {
        case WINTER:
            this->tempRange.min = 0;
            this->tempRange.max = 200;
            this->humidityRange.min = 0;
            this->humidityRange.max = 200;
            break;
        case GERMINATING:
            this->tempRange.min = 18;
            this->tempRange.max = 32;
            this->humidityRange.min = 70;
            this->humidityRange.max = 95;
            break;
        case VEGATATIVE:
            this->tempRange.min = 20;
            this->tempRange.max = 25;
            this->humidityRange.min = 60;
            this->humidityRange.max = 70;
            break;
        case FRUITING:
            this->tempRange.min = -200;
            this->tempRange.max = 28;
            this->humidityRange.min = 40;
            this->humidityRange.max = 50;
            break;
        case NIGHT:
            this->tempRange.min = 15;
            this->tempRange.max = 30;
            this->humidityRange.min = 30;
            this->humidityRange.max = 80;
            break;
        }
    }
    int getTemp()
    {
        return this->currentTemp;
    }
    int getHumidity()
    {
        return this->currentHumidity;
    }

private:
    int currentTemp;
    int currentHumidity;

    Boundaries tempRange;
    Boundaries humidityRange;
};

// Function to state enums to strings
String stateToString(State state)
{

    String stateToReturn = "";

    switch (state)
    {
    case TOO_HIGH:
        stateToReturn = "TOO HIGH";
        break;
    case LEVELS_OK:
        stateToReturn = "LEVELS_OK";
        break;
    case TOO_LOW:
        stateToReturn = "TOO LOW";
        break;
    }
    return stateToReturn;
}

class LED
{
private:
    RGBLed *led;
    long lastChange;

    bool flash = false;
    State soilState, envState;

public:
    LED(RGBLed *actualLed)
    {
        led = actualLed;
        lastChange = 0;
    }

    void setSystemState(State soilState, State envState)
    {
        this->soilState = soilState;
        this->envState = envState;
    }

    void update()
    {
        // Show relevant output dependent on component states
        if (envState == LEVELS_OK && soilState == LEVELS_OK)
        {
            led->setColor(RGBLed::GREEN);
        }
        else
        {

            if (soilState != LEVELS_OK)
            {
                if (millis() - lastChange > BLINK_RATE)
                {
                    lastChange = millis();
                    flash = !flash;
                }
            }
            else
                flash = false;

            if (flash)
            {
                led->setColor(RGBLed::BLUE);
            }
            else
            {
                if (envState != LEVELS_OK)
                    led->setColor(RGBLed::RED);
                else
                    led->off();
            }
        }
    }
};