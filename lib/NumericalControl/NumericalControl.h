#ifndef NUMERICALCONTROL_H
#define NUMERICALCONTROL_H

#include <ESP32Encoder.h>
#include <Arduino.h>
#include <StorageManager.h>

class NumericalControl{

public:

    NumericalControl(int clk,int dt, int button){

          this->encoder.attachHalfQuad(dt, clk);
          this->encoder.setCount(0);
          this->buttonPin =  button; 
          pinMode(button, INPUT_PULLUP);


    }

    void setBoundaries(int min, int max){

        this->rotation.min = min;
        this->rotation.max = max;
    }

    void update(){

        prevPosition = currentPosition;

        long newPosition = abs(encoder.getCount() / 2);

        if (newPosition > this->rotation.max)
            newPosition =  this->rotation.max;
        else if (newPosition <  this->rotation.min)
            newPosition = this->rotation.min;

        if (newPosition != this->currentPosition)
        {
            currentPosition = newPosition;
            Serial.println(newPosition);
        }

        if (digitalRead(this->buttonPin) == LOW)
        {   
            this->buttonPressed = true;
        }
        else{
            this->buttonPressed = false;
        }
        delay(50);
    }

    bool wasPressed(){
        return this->buttonPressed;
    }

    int poll(){
        return this->currentPosition;
    }

    bool scroll(){
        return currentPosition != prevPosition; // change to return direction -1 for back , 1 for forward , 0 for no action??
    }



protected:

ESP32Encoder encoder;
int buttonPin;
MinMax rotation;
int currentPosition = 0;
int prevPosition = 0;
bool buttonPressed = false;
bool messageSent = false;
long lastSent = 0;


};




#endif