# Cold-Chain-Monitor

## What does it do?
Monitors temperature and humidity of medicine storage in real time using ESP32 and DHT22 sensor.
If temperature goes outside the safe range, it instantly sends a Telegram alert so action can be taken before medicines get damaged.

## Why I built this
Medicine storage is a real problem in India — small clinics and pharmacies often have no way to know if their storage temperature is safe.
I built this as a low cost IoT solution that anyone can set up with basic hardware.

## Hardware used
- ESP32
- DHT22 Temperature and Humidity Sensor

## Features
- Reads temperature and humidity every 10 seconds
- Sends instant Telegram alert if temperature goes out of safe range
- Live web dashboard accessible from any device on same network
- Full data log with timestamps

## How to run
1. Open main.cpp in Arduino IDE
2. Enter your WiFi name and password
3. Enter your Telegram username and CallMeBot API key
4. Upload to ESP32
5. Open Serial Monitor to get the IP address
6. Open that IP in your browser

## Built by
Gauresh Pathak — Electronics and Telecommunication Engineering Student, Mumbai

## Future Improvements
- Add cloud storage using Firebase
- Mobile app for remote monitoring
- Support for multiple storage units
- Battery backup with solar charging
