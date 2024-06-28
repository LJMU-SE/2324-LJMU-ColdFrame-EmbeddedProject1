#include <Components.h>
#include <StorageManager.h>
#include <DisplayScreen.h>
#include <NumericalControl.h>
#include <ModeManager.h>
#include <OnlineStorage.h>

// Initialise sensor objects and pins
int soilPin = 33;
int dhtPin = 16;
Enviroment dht11(dhtPin);
SoilSensor soilSensor(soilPin);

// Encoder pins
#define ENCODER_CLK 34 
#define ENCODER_DT 35  
#define ENCODER_BUTTON 32

NumericalControl numericalControl(ENCODER_CLK,ENCODER_DT,ENCODER_BUTTON);

// Led pins
int redPin = 15;
int greenPin = 4;
int bluePin = 13;
RGBLed led(redPin, greenPin, bluePin, RGBLed::COMMON_CATHODE);
LED realLed(&led);

// Keeps track of current operating mode
Mode* currentMode;

// Keeps track of sensors states
State environmentState;
State soilState;
Readings readings;

// Display and storage objects
ModeManager modeManager = ModeManager();
StorageManager storageManager = StorageManager(&modeManager);
DisplayScreen* display;

OnlineStorage* onlineStorage;

// Delay time between readings
const long READINGS_DELAY = 2000;
unsigned long readingsLastChange = 0;

void setup()
{

  Serial.begin(115200);

  // Initialise display with a reference to encoder to drive menu changes
  display = new DisplayScreen(&numericalControl);

  onlineStorage = new OnlineStorage("espWifi","ljmu1111");
}

void loop()
{

  unsigned long currentMillis = millis();

  currentMode = modeManager.getCurrentMode();
  soilSensor.setMode(currentMode);
  dht11.setMode(currentMode);

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

    onlineStorage->tick(readings,currentMode);

    readingsLastChange = currentMillis;

  }

  // Update LED Status
  realLed.setSystemState(soilState, environmentState);

  display->update(readings,&storageManager);

  storageManager.storeLastRead(readings);
  storageManager.tick();

}
