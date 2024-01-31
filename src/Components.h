#include <Arduino.h>
#include <dht.h>

// Stores operating modes
enum Mode{
    WINTER,
    GERMINATING,
    VEGATATIVE,
    FRUITING,
    NIGHT
};

// Stores state indicators
enum State{
    TOO_HIGH,
    OK,
    TOO_LOW
};

// Stores sensor ranges
struct Boundaries{
    int min;
    int max;
};

// Allows for DHT sensor reading
dht DHT;

// Parent class for components
class Component {
public:

    // Constructor
    Component(Mode initialMode, int pin) {
        this->pin = pin;
        this->operatingMode = initialMode;
    }

    // Updates value and checks if state needs to change
    virtual void update() = 0;

    // Sets mode and changes boundaries accordingly   
    virtual void setMode(Mode newMode) = 0;

protected:
    int pin;
    Mode operatingMode;
    State status;
};

// Child class for Soil Sensor
class SoilSensor : public Component{
public:

    // Constructor
    SoilSensor(Mode initialMode, int pin) : Component(initialMode,pin) {}

    // Returns current moisture level
    int getValue(){
        return this->moistureLevel;
    }

    // Sets mode and changes boundaries accordingly
    virtual void setMode(Mode newMode) override {
        this->operatingMode = newMode;
        
        switch (this->operatingMode){
            case WINTER:
                break;
            case GERMINATING:
                break;
            case VEGATATIVE:
                break;
            case FRUITING:
                break;
            case NIGHT:
                break;                
        }
        update();
    }

    // Updates value and checks if state needs to change
    virtual void update() override{
        this->moistureLevel = analogRead(this->pin);
    }

protected:
    int moistureLevel;
    Boundaries moistureLimit;
};

// Child class for DHT sensor
class Enviroment : public Component{
public:

    // Constructor
    Enviroment(Mode initialMode, int pin) : Component(initialMode,pin) {}

    // Updates temp and humidity 
    virtual void update() override {
        DHT.read11(this->pin);
        this->currentTemp = DHT.temperature;
        this->currentHumidity = DHT.humidity;
    }

    // Sets mode and changes boundaries accordingly
    virtual void setMode(Mode newMode){
        this->operatingMode = newMode;

        switch (this->operatingMode){
            case WINTER:
                break;
            case GERMINATING:
                break;
            case VEGATATIVE:
                break;
            case FRUITING:
                break;
            case NIGHT:
                break;                
        }
        update();
    }


private:
    int currentTemp;
    int currentHumidity;

    Boundaries tempRange;
    Boundaries humidityRange;
};


