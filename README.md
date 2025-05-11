# Smart Water Tank System

An IoT-based Smart Water Tank Management System built with ESP32, designed to monitor water levels, usage, and quality, and automate valve control. The system provides real-time data and remote monitoring via MQTT and visualizes metrics using a cloud database and dashboard.


## Features

- **Ultrasonic Sensor**: Measures current water level
- **Flow Sensor**: Monitors water usage over time
- **Turbidity Sensor**: Detects water clarity (quality)
- **RGB LED**: Visual status indicators (e.g., full, low, dirty)
- **Solenoid Valve**: Automatically controls water inflow
- **OLED Display (U8G2)**: Displays real-time tank status
- **MQTT Integration**: Sends data to broker for remote monitoring
- **MongoDB Atlas**: Stores historical data for analysis
- **Dashboard Support**: Integrates with Node-red for real-time visuals and PowerBI for historic trends visualization
- **Predictive Model**: Models developed from the data collected through cloud DB


## Hardware Components

- ESP32 DevKit v1
- HC-SR04 Ultrasonic Sensor
- YF-S201 Flow Sensor
- Turbidity Sensor
- RGB LED
- 12V Solenoid Valve
- 5V Relay
- OLED Display (0.96" I2C, U8G2)
- Power Supply (e.g., 12V adapter)
- Relay Module (for solenoid control)


## Data Flow

1. Sensors collect data (level, flow, quality)
2. Real time visualisation using Node-red
3. ESP32 processes data and sends it via MQTT
4. Backend script stores data in MongoDB Atlas
5. Dashboard (Power BI) visualizes data trends
6. Thresholds trigger LED changes and valve control

## Structural Diagram
![system diagram](https://github.com/user-attachments/assets/95a942e9-0259-498c-b8b3-a697da1072e2)

## Circuit Diagram 
![cuircuit diagram](https://github.com/user-attachments/assets/b92691dc-af7c-44b3-ac2e-ab8ad7c17f86)

## Dashboard

### Historic Data trend
![powerBI](https://github.com/user-attachments/assets/a3a1897a-04ce-40c9-9458-7a6560882acc)

### Real-time
<img width="1417" alt="dashboard" src="https://github.com/user-attachments/assets/36434c5a-a939-4ad8-b55b-46ec7f687feb" />
![phone dashboard 2](https://github.com/user-attachments/assets/ef81f850-100a-4224-85ab-cae31c9289c4)
