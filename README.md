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






