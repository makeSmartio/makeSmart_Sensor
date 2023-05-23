int ThisRockVersion = 91;
String AlphaOrBeta = "Alpha";
int NotifyEverySeconds = 60;
int pirCount = 0;
const int myInterval = 500;
const char* host = "makeSmart.io";
const int httpPort = 80;
String SensorName;
String sensorMode = "Blank";
String mac_addr;

const char* apPassword = "asdfasdf";
long rssi;

bool keepDataPoint = 0;
bool sendMotionAlerts = 0;
bool trackMotion = 1;
int secsSinceLastSend = 100;
int sendFrequency = 300;
int secsSinceLastAlert;
#include <EEPROM.h>
#include <Time.h>
#include <TimeLib.h>
time_t lastGyroError = now() - 10000;
time_t lastTooColdAlert = now() - 10000;
time_t lastTooHotAlert = now() - 10000;
time_t lastWaterAlert = now() - 10000;
time_t lastSend = now() - 10000;
time_t lastBmeRead = now() - 10000;
time_t lastAlertTime = now() - 10000;
time_t lastMotionAlert = now() - 1000;
time_t lastSoilAlert = now() - 1000;
time_t lastAirPolutionAlert = now() - 1000;

time_t dryerRunningSince;
int dryerSensitivity = 1000;
int dryerStarting = 0;
bool dryerRunning = false;
int dryerStopping = 0;
int minDryerCycleTime = 600; //10 minutes
time_t washerRunningSince;
int washerSensitivity = 1500;
int washerStarting = 0;
bool washerRunning = false;
int washerStopping = 0;
int minCycleTime = 300;

float warnAboveDHTTemp = 100;
float warnBelowDHTTemp = -100;
float warnAboveProbe1 = 100;
float warnBelowProbe1 = -100;
float warnAboveTemp2 = 100;
float warnBelowTemp2 = -100;
String webSite, javaScript, JSON;

bool relay1State;

int relay1OnTemp = 78;
int relay1OffTemp = 75;

#include <ArduinoJson.h>

#include <DNSServer.h>

#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager


#include <WiFiClient.h>
WiFiClient client;
String ipAddr;


#if defined(ESP8266)
  #include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
  #include <ESP8266WebServer.h>
  #include <ESP8266HTTPUpdateServer.h>
  ESP8266WebServer httpServer(80);
  //httpServer.begin();
  //ESP8266WebServer.begin(client, httpServer);
  
  ESP8266HTTPUpdateServer httpUpdate;
  #include <ESP8266httpUpdate.h>
  #include <ESP8266mDNS.h>
  //#include <ESP8266HTTPClient.h>
 
  #define OLED_RESET   D4//  4 // Reset pin # (or -1 if sharing Arduino reset pin)
  #define ONE_WIRE_BUS_1 D6 //D6 on NodeMCU
  int drySoilValue = 400;
  int relay1Pin = D4;
  int waterPresenceVal = 400;
#else
  #include <WiFi.h>          //https://github.com/esp8266/Arduino
  #include <WebServer.h>
  WebServer httpServer(80);
  #include <HTTPUpdate.h>
  //#include <ESP32httpUpdate.h>
  #include <Update.h>
  #include <ESPmDNS.h>
  #define OLED_RESET   -1 //2 // Reset pin # (or -1 if sharing Arduino reset pin)
  int D7 = 13;
  int D5 = 5; 
  int A1 = 33;
  int D2 = 2;
  int D4 = 4;
  int relay1Pin = 4;
  #define ONE_WIRE_BUS_1 15
  int drySoilValue = 3000;
  int waterPresenceVal = 2000;
#endif

#include <OneWire.h>

#include "Adafruit_BME680.h"
Adafruit_BME680 BME680; // I2C


#include <BME280I2C.h>
//#include <Adafruit_BME280.h>
//Adafruit_BME280 bme280;
BME280I2C bme;    // Default : forced mode, standby time = 1000 ms
                  // Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off,

#include "Adafruit_Si7021.h" //legacy sensors. the BME chip is better
Adafruit_Si7021 si7021 = Adafruit_Si7021();
bool si7021onBoard;
float siTemp(NAN), siHumi(NAN);

#include <DallasTemperature.h>
OneWire oneWire_in(ONE_WIRE_BUS_1);
DallasTemperature DS18B20_Sensor(&oneWire_in);



#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


//#include "I2Cdev.h"
#include "MPU6050.h"

bool metric = false;
double tcTemp;

int NoDhtData = 0;

int ledPin = 2; //D4

//I started this project using the popular DHT sensor. I moved away from it, but the nameing remians for now. Sorry!
time_t lastDHTread;
float Dht22Humi, Dht22Temp = -196;  // Values read from sensor
float bmeTemp(NAN), bmeHumi(NAN), bmePres(NAN);
float oldTemp;
time_t oldTempTime = now();
//ADC_MODE(ADC_VCC); //vcc read
//#define ESP_INIT_DATA_ENABLE_READADC

String apName;
MPU6050 accelgyroIC1(0x68);

int i = 0;
int d = 0;
bool first100LoopCompleted = false;
int16_t accelArray[100];
int16_t gyroArray[100];
float accelArrayAvg, gyroArrayAvg;
int16_t ax, ay, az, gx, gy, gz, accel1, gyro1;
int16_t axb, ayb, azb, gxb, gyb, gzb;
int16_t axa, aya, aza, ax1, ay1, az1;
int16_t gxa, gya, gza, gx1, gy1, gz1;
float AccelTemp, Probe1, Probe2;
String chipId;
char apNameCharBuf[100];

long vdd = 0;
int16_t analogVal;
bool bme280onBoard;
bool bme680onBoard;
String webPage;
String line1;//, line2, line3, line4, line5;

#include <Adafruit_ADS1015.h>
Adafruit_ADS1115 ads;  // Declare an instance of the ADS1115
float scalefactor = 0.1875F; // This is the scale factor for the default +/- 6.144 Volt Range we will use
#define                         THERMISTOR_COUNTS 10                   // size of thermistor input averaging array
double                        dKelvin = 273.15;                       // degrees kelvin
// Voltage Divider resistor.
double                        dResistor = 100000;                      // in ohms
// Steinhart-Hart coeffecients from the spreadsheet.
double                        dProbeA = 4.3989177E-05;            // value A from spreadsheet
double                        dProbeB = 2.6924278E-04;             // value B from spreadsheet
double                        dProbeC = -8.5735460E-08;            // value C from spreadsheet

// Temperature.
double                        dDegreesC = 0.0;                        // calculated degrees C
double                        dDegreesF = 0.0;                        // calculated degrees F

// Thermistor.
double                        dThermistorAverage = 0.0;               // average of all thermistor counts in nThermistorCounts[]
int                           nThermistorCounts[THERMISTOR_COUNTS];   // input counts array used for averaging
int                           nThermistorPointer = 0;                 // location of next input in nThermistor array

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
