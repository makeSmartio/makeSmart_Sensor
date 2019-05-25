# makeSmart Sensor

Arudunio code for the multi-purpose sensor sold on Tindie and eBay. The sensor is designed to log data and alert for: water presence, too hot, too cold, motion, washing mashing machine/dyer finished cycle, and soil moisture.

## Getting Started

You will need to download and install the Arduino IDE add the board manager lines.
IDE: https://www.arduino.cc/en/Main/Software
How to install board managers: https://arduino-esp8266.readthedocs.io/en/latest/installing.html
ESP8266: https://arduino.esp8266.com/stable/package_esp8266com_index.json
ESP32: https://dl.espressif.com/dl/package_esp32_index.json

Then use the Arduino Library Manager to install the following libraries:
Time
WifiManager by T
ArduinoJson //version 5 for now
OneWire
BME280 by Tyler Glen
Adafruit_Si7021 //I prefer the BME280 chip, but some old sensors have the also good Si7021 temperature sensor
DallasTemperature
MPU6050 

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags). 

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Huge thanks to tablatronix and everyone at the https://github.com/tzapu/WiFiManager library. I wouldn't have done this without this library
* All of the YouTubers and folks who have written how to's, and help guides on the web
