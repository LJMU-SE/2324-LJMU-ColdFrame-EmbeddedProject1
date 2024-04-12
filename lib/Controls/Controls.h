#include <Arduino.h>
#include <ESP32Encoder.h>
#ifndef CONTROLS_H
#define CONTROLS_H


#define ROTARY_BUTTON_PIN 22
#define ROTARY_CLK 13
#define ROTARY_DT 21


class Control{
public:

    Control(){

        encoder.attachHalfQuad (ROTARY_DT,ROTARY_CLK);
        encoder.setCount(0);

        while(true){
            long newPosition = encoder.getCount() / 2;
            Serial.println(newPosition);
            delay(100);
        }
            
}
 
  

         

    

private:

ESP32Encoder encoder;

};
#endif    