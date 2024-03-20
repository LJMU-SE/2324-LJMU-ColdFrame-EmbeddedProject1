#include <Arduino.h>
#include <DHTesp.h>
#include <RGBLed.h>
#include <Modes.h>

// Stores state indicators
enum State
{
    TOO_HIGH,
    LEVELS_OK,
    TOO_LOW
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

    // Sets mode and changes MinMax accordingly
    virtual void setMode(Mode newMode) = 0;

    // Returns current state
    virtual State getStatus()
    {
        return this->status;
    }

    virtual int getPin()
    {
        return this->pin;
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

    virtual void setMode(Mode newMode) override
    {
        this->moistureLimit = newMode.getSoilRange();
    }

    int getMoisturePercent()
    {
        return this->moisturePercent;
    }

protected:
    MinMax moistureLimit;
    int moistureLevel;
    u_int8_t moisturePercent;
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


    virtual void setMode(Mode newMode) override {
        this->humidityRange = newMode.getHumidityRange();
        this->tempRange = newMode.getTempRange();
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

    MinMax tempRange;
    MinMax humidityRange;
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