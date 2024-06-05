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

    void update(){

        prevPosition = currentPosition;

        long newPosition = encoder.getCount() / 2;

        // if (newPosition > this->rotation.max)
        //     newPosition =  this->rotation.max;
        // else if (newPosition <  this->rotation.min)
        //     newPosition = this->rotation.min;

        if (newPosition != this->currentPosition)
        {
            currentPosition = newPosition;
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

    bool scroll(){
        return currentPosition != prevPosition; 
    }

    int getDirection(){

        int dir = 0;

        if (currentPosition > prevPosition){
            dir = -1;
        }
        if (currentPosition < prevPosition){
            dir = 1;
        }
        return dir;
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