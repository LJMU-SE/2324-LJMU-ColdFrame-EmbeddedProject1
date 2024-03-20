#include <Components.h>
#include <ESP32Servo.h>
#include <StorageManager.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#define TFT_CS  27
#define TFT_RST  14 
#define TFT_DC   17
#define TFT_MOSI 23  
#define TFT_SCLK 15  
void printText();


// Initialise sensor objects and pins
int soilPin = 26;
int dhtPin = 16;

Enviroment dht11(dhtPin);
SoilSensor soilSensor(soilPin);

// Led pins
int redPin = 18;
int bluePin = 13;
int greenPin = 4;
RGBLed led(redPin, greenPin, bluePin, RGBLed::COMMON_CATHODE);
LED realLed(&led);

Mode winterMode(ModeType::WINTER, 0, 200, 0, 200, 0, 20);
Mode germMode(ModeType::GERMINATING, 18, 32, 70, 95, 0, 60);
Mode vegMode(ModeType::VEGATATIVE, 20, 25, 60, 70, 40, 80);
Mode fruitMode(ModeType::FRUITING, -200, 28, 40, 50, 40, 80);
Mode nightMode(ModeType::NIGHT, 15, 30, 30, 80, 0, 20);

// Keeps track of current operating mode // maybe make a pointer
Mode currentMode;

// Keeps track of sensors states
State environmentState;
State soilState;

Readings readings;
StorageManager storageManager = StorageManager();  

// Delay time between debug string
const long DEBUG_DELAY = 5000;
const long READINGS_DELAY = 2000;
unsigned long debugLastChange = 0;
unsigned long readingsLastChange = 0;

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);


void setup()
{
  // Set initial operating mode and system state
  currentMode = vegMode;
  // Initialise serial communication
  Serial.begin(115200);
  // Construct sensors with starting mode and pin
  soilSensor.setMode(currentMode);
  dht11.setMode(currentMode);

  // Initialise output display
  tft.initR(INITR_144GREENTAB); 
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

    // Store readings
    readings.temp = dht11.getTemp();
    readings.hum = dht11.getHumidity();
    readings.moist = soilSensor.getMoisturePercent();
    readings.envState = stateToString(environmentState);
    readings.soilState = stateToString(soilState);

    
    // Update persistant data storage 
    storageManager.updateBuffer(readings);

  }

  realLed.setSystemState(soilState, environmentState);

  realLed.update();

  // Check if delay time has passed
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
    printText();
  }
}


void printText(){

        // initialise the display
        tft.setFont();
        tft.fillScreen(ST77XX_BLACK);
        tft.setTextColor(ST77XX_MAGENTA);
        tft.setTextSize(1);
        
        tft.setCursor(0,0);
        tft.print("Latest Readings\n");
        tft.printf("Humidity %d\n",readings.hum);
        tft.printf("Temperature %d\n",readings.temp);
        tft.printf("Soil Moisture %d\n",readings.moist);
        tft.printf("Soil State %s\n",readings.soilState);
        tft.printf("Environment State %s\n",readings.envState);
}