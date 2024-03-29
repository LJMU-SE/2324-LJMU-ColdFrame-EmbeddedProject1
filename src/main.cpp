#include <Components.h>
#include <StorageManager.h>
#include <DisplayScreen.h>

// Initialise sensor objects and pins
int soilPin = 26;
int dhtPin = 16;
Enviroment dht11(dhtPin);
SoilSensor soilSensor(soilPin);

// Led pins
int redPin = 18;
int greenPin = 4;
int bluePin = 13;
RGBLed led(redPin, greenPin, bluePin, RGBLed::COMMON_CATHODE);
LED realLed(&led);

Mode winterMode(ModeType::WINTER, 0, 200, 0, 200, 0, 20);
Mode germMode(ModeType::GERMINATING, 18, 32, 70, 95, 0, 60);
Mode vegMode(ModeType::VEGATATIVE, 20, 25, 60, 70, 40, 80);
Mode fruitMode(ModeType::FRUITING, -200, 28, 40, 50, 40, 80);
Mode nightMode(ModeType::NIGHT, 15, 30, 30, 80, 0, 20);

// Keeps track of current operating mode 
Mode currentMode;

// Keeps track of sensors states
State environmentState;
State soilState;
Readings readings;

// Display and storage objects
StorageManager storageManager = StorageManager();  
DisplayScreen display;

// Delay time between readings
const long READINGS_DELAY = 2000;
unsigned long readingsLastChange = 0;


void setup()
{

  Serial.begin(115200);

  // Construct sensors with starting mode and pin
  currentMode = vegMode;
  soilSensor.setMode(currentMode);
  dht11.setMode(currentMode);

  // Initialise display
  display = DisplayScreen();
}

void loop()
{

  unsigned long currentMillis = millis();

  if (currentMillis - readingsLastChange >= READINGS_DELAY)
  {
    // Update sensors and check their states
    dht11.update();
    soilSensor.update();
    environmentState = dht11.getStatus();
    soilState = soilSensor.getStatus();

    // Store readings
    readings.temp = dht11.getTemp();
    readings.hum = dht11.getHumidity();
    readings.moist = soilSensor.getMoisturePercent();
    readings.envState = stateToString(environmentState);
    readings.soilState = stateToString(soilState);

    readingsLastChange = currentMillis;
  }

  // Update LED Status
  realLed.setSystemState(soilState, environmentState);
  realLed.update();

  display.update(readings,&storageManager);

  storageManager.storeLastRead(readings);
  storageManager.update();  
}


