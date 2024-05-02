# Cold Frame Embedded Systems Project

<img src="https://andersonquigley.com/content/uploads/2022/11/ljmu_logo.png" alt="LJMU Banner Logo" width="100%" />

# Garden Cold Frame Monitoring System

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
- **User Editable Mode** (Not yet implemented)

## Features

### Implemented Features

#### Basic Loop & Data Logging
- Reads sensors as fast as possible.
- Outputs a debug string to the Serial output every 5 seconds.
- The debug string includes the latest values for all sensor readings and system statuses, detailing whether sensors are either above, within, or below the set range.

#### User Output Display
- Integrated output display screen that reports current sensor values and statuses.
- Displays minimum and maximum temperature/humidity over the last 24 hours.

### Features Not Yet Implemented
- **Mode Selection**
- **User Editable Mode**
- **Responsive HVAC Control**
- **Remote Storage**
- **Alerts**
- **Remote Control and OTA Updates**
- **NVM Storage**
- **Power Management**


