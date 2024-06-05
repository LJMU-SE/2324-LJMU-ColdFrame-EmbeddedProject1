# Cold Frame Embedded Systems Project

<img src="https://andersonquigley.com/content/uploads/2022/11/ljmu_logo.png" alt="LJMU Banner Logo" width="100%" />

## Project Objective
The aim of this project is to develop an embedded system for operating as a garden cold frame monitoring system. 

## Hardware
The system is developed using the **ESP-32 microcontroller**. The user interface is facilitated through an **Adafruit TFT display**, which provides real-time feedback on the current environment and system status.

## Operating Modes
The system features multiple operating modes tailored to various stages of plant development:
- **Winter**
- **Germinating**
- **Vegetative**
- **Fruiting**
- **Night**
- **User Editable Mode** 

## Features

### Implemented Features

#### Basic Loop & Data Logging
- Reads sensors as fast as possible.
- Outputs a debug string to the Serial output every 5 seconds.
- The debug string includes the latest values for all sensor readings and system statuses, detailing whether sensors are either above, within, or below the set range.

#### User Output Display
- Integrated output display screen that reports current sensor values and statuses.
- Displays minimum and maximum temperature/humidity over the last 24 hours.

#### Mode Selection
- Through the use of a rotary encoder, the user is now able to change the operating mode of the system.
- Display has been updated to show value states using colour indicators , with values being printed in red if they are out of the defined range of the current operating mode, and green if they are within.

https://github.com/LJMU-SE/cold-frame-project/assets/150610221/343fb669-dabe-4b0e-a036-9ad7c4bdfc85

#### Custom User Mode
 - User is now able to create their custom operating mode and select it in the mode list.
 - Double buffering has now been added for a smoother display, eliminating flickering when the screen updates.

 
### Features Not Yet Implemented
- **Responsive HVAC Control**
- **Remote Storage**
- **Alerts**
- **Remote Control and OTA Updates**
- **NVM Storage**
- **Power Management**

### Setup Images
As of 02/05/24
![PXL_20240327_142814390](https://github.com/LJMU-SE/Cold-Frame-Project/assets/150610221/50ae2483-8dfc-4a61-834f-6fdcb3a53ad6)

As of 30/05/24
![PXL_20240530_111212887](https://github.com/LJMU-SE/cold-frame-project/assets/150610221/cd6fbb14-c595-46ea-913f-c4c3936e561b)






