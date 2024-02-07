#include <Components.h>

// Initialise sensor objects and pins
int soilPin = 27;
int dhtPin = 16;

SoilSensor soilSensor(soilPin); 
Enviroment dht11(dhtPin);

// Led pins
int redPin = 18;
int bluePin = 15;
int greenPin = 4;
RGBLed led(redPin, greenPin, bluePin, RGBLed::COMMON_CATHODE);
LED realLed(&led);

// Keeps track of current operating mode
Mode currentMode = VEGATATIVE;

// Keeps track of sensors states
State environmentState;
State soilState;

// Keeps track of debug values
int temperature;
int humidity;
int moistureLevel;

// Delay time between debug string
const long DEBUG_DELAY = 5000;
const long READINGS_DELAY = 2000;
unsigned long debugLastChange = 0;
unsigned long readingsLastChange = 0;

void setup()
{

  // Set initial operating mode and system state
  currentMode = VEGATATIVE;

  // Initialise serial communication
  Serial.begin(115200);

  // Construct sensors with starting mode and pin
  soilSensor.setMode(currentMode);
  dht11.setMode(currentMode);
}

void loop()
{

  // Get current milliseconds
  unsigned long currentMillis = millis();


  if (currentMillis - readingsLastChange >= READINGS_DELAY)
  {
    // Update sensors and check their states
    dht11.update();
    soilSensor.update();
    environmentState = dht11.getStatus();
    soilState = soilSensor.getStatus();

    readingsLastChange = currentMillis;

    // Get readings
    temperature = dht11.getTemp();
    humidity = dht11.getHumidity();
    moistureLevel = soilSensor.getMoisturePercent();
  }

  realLed.setSystemState(soilState, environmentState);

  realLed.update();

  // Check if delay time has passed
  if (currentMillis - debugLastChange >= DEBUG_DELAY)
  {
    debugLastChange = currentMillis;

    Serial.printf("Debug String \n");
    Serial.printf("-------------------------------------------------------------\n");
    Serial.printf("|| Temperature %d° || Humidity %d%% || Environment State: %s \n", temperature, humidity, stateToString(environmentState));
    Serial.printf("-------------------------------------------------------------\n");
    Serial.printf("|| Soil Moisture: %d%% || Soil Moisture State: %s\n", moistureLevel, stateToString(soilState));
    Serial.printf("-------------------------------------------------------------\n");
    Serial.println("\n");
  }
}


