#include <Components.h>

// Initialise sensors and pins
SoilSensor soilSensor;
int soilPin = 36;

Enviroment dht11;
int dhtPin = 36;

// Keeps track of current operating mode
Mode currentMode;


void setup() {

  // Set initial operating mode
  currentMode = VEGATATIVE;
  
  // Construct sensors with starting mode and pin
  soilSensor = SoilSensor(currentMode,36);
  dht11 = Enviroment(currentMode,36);


}

void loop() {
  
}

