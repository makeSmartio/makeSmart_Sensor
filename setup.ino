void setup() {
  Serial.begin(115200);
  //Serial.setDebugOutput(true);
  Wire.begin();
  
  String resetReason;
  
#if defined(ESP8266)
  Serial.println(ESP.getResetInfo());
  rst_info *rinfo;
  rinfo = ESP.getResetInfoPtr();
  if ((*rinfo).reason == 0)
  {resetReason = "Power cycle";}
  else if ((*rinfo).reason == 1)
  {resetReason = "Hardware WDT reset";}
  else if ((*rinfo).reason == 2)
  {resetReason = "Fatal exception";}
  else if ((*rinfo).reason == 3)
  {resetReason = "Software watchdog reset";}
  else if ((*rinfo).reason == 4)
  {resetReason = "Software reset";}
  else if ((*rinfo).reason == 5)
  {resetReason = "Deep-sleep";}
  else if ((*rinfo).reason == 6)
  {resetReason = "Hardware reset";}
  else
  {resetReason = String("ResetInfo.reason = ") + (*rinfo).reason;  }
  Serial.println(resetReason);
  
  chipId = ESP.getChipId();
  Serial.println(ESP.getChipId());
  //chipId.trim();
  if (chipId.length() < 3)
  {
    Serial.println("Blank");
    chipId = WiFi.macAddress();
    chipId.replace(":","");
  }
  
  Serial.print("ESP8266 ChipId: ");
  Serial.println(chipId);
  
  
  httpUpdate.setup(&httpServer);

  pinMode(D7, OUTPUT); //NodeMCU PCB uses D7 for 3.3v for the DS18B20 sensors
  digitalWrite(D7, HIGH);


#else
  esp_reset_reason_t reason = esp_reset_reason();

  Serial.println(reason);
  if (reason==ESP_RST_UNKNOWN)
    {resetReason = "Reset reason can not be determined";}
  else if (reason==ESP_RST_POWERON)
    {resetReason = "Reset due to power-on event";}
  else if (reason==ESP_RST_EXT)
    {resetReason = "Reset by external pin";}
  else if (reason==ESP_RST_SW)
    {resetReason = "Software reset via esp_restart";}
  else if (reason==ESP_RST_PANIC)
    {resetReason = "Reset (software or hardware) due to interrupt watchdog";}
  else if (reason==ESP_RST_INT_WDT)
    {resetReason = "Reset due to task watchdog";}
  else if (reason==ESP_RST_INT_WDT)
    {resetReason = "ESP_RST_INT_WDT";}
  else if (reason==ESP_RST_WDT)
    {resetReason = "Reset due to other watchdogs";}
  else if (reason==ESP_RST_DEEPSLEEP)
    {resetReason = "Reset after exiting deep sleep mode";}
  else if (reason==ESP_RST_BROWNOUT)
    {resetReason = "Brownout reset (software or hardware)";}
  else if (reason==ESP_RST_SDIO)
    {resetReason = "Reset over SDIO";}
  else
    {resetReason=reason;}
  
  uint64_t chipid = ESP.getEfuseMac(); // The chip ID is essentially its MAC address(length: 6 bytes).
  uint16_t chip2 = (uint16_t)(chipid >> 32);
  char chip3[23];
  snprintf(chip3, 23, "%04X%08X", chip2, (uint32_t)chipid);
  chipId = String(chip3);
  Serial.print("ESP32 ChipId: ");
  Serial.println(chipId);

  pinMode(D4, OUTPUT);//ESP32 Dev Board uses D4 for 3.3V on the DS18B20 mini-pin jack
  digitalWrite(D4, HIGH);
  
  pinMode(19, OUTPUT);//ESP32 Dev Board uses D19 for GND on the A0 mini-pin jack
  digitalWrite(19, LOW);
  
#endif  

  EEPROM.begin(64);
  delay(100);
  String EEPROMData = "";
  EEPROMData = readEEPROM(10);
  if (EEPROMData!="")
  {resetReason = EEPROMData;}
  writeEEPROM(10,""); //clear old data
  Serial.print("EEPROM Data:");
  Serial.println(resetReason);
  Serial.flush();
  Serial.println(ThisRockVersion);  

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println("SSD1306 allocation failed");
  }
  Serial.println("SSD1306 allocation worked");
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner

  display.clearDisplay();
  display.display();

  display.println("makeSmart(startUp)");
  display.display();


  
  SensorName = readEEPROM(0);
  Serial.print("eeprom SensorName: ");
  Serial.println(SensorName);
  
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //wifiManager.setDebugOutput(true); 
  //Serial.println(WiFi.SSID());
  //Serial.println(WiFi.psk());
  
  //WiFi.printDiag(Serial);

  //use button to reset wifi settings
  pinMode(D5, INPUT_PULLUP);
  Serial.println("D5: ");
  int d5 = digitalRead(D5);
  Serial.println(d5);
  if (d5 == 0)
  {
    display.println("Resetting Wifi Settings");
    display.display();
    //delay(3000);
    //wifiManager.autoConnect();
    resetWifi();
    //ESP.eraseConfig();
    //WiFi.disconnect(false,true); 
    display.println("Done. Please power cycle.");
    display.display();
    ESP.restart();
  }

  mac_addr = WiFi.macAddress();
  Serial.println(mac_addr);

  wifiManager.setTimeout(180);

  wifiManager.setCustomHeadElement("<div>Welcome to makeSmart()</div>");

  String chipString = "makeSmart-" + chipId;
  const char* chipChar = chipString.c_str();
  WiFiManagerParameter custom_text(chipChar);
  wifiManager.addParameter(&custom_text);
    
  if (SensorName == "")
    apName = "makeSmart";
  else
    apName = "makeSmart";//-" + SensorName;
  
  apName.toCharArray(apNameCharBuf, 100);

  display.print("Connect your wifi to:\n" + String(apNameCharBuf));
  display.display();

  wifiManager.setAPStaticIPConfig(IPAddress(10, 10, 10, 10), IPAddress(), IPAddress(255, 255, 255, 0));
  ////wifiManager.setSTAStaticIPConfig(IPAddress(10,10,10,10), IPAddress(), IPAddress(255,255,255,0));

  wifiManager.autoConnect(apNameCharBuf, apPassword);
  //wifiManager.autoConnect();

  //if you get here you have connected to the WiFi
  
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Connected to Wifi");
  display.display();
  Serial.println("Connected...");

  Serial.println("Local IP Address:");
  ipAddr =  String(WiFi.localIP()[0]) + "." + String(WiFi.localIP()[1]) + "." + String(WiFi.localIP()[2]) + "." + String(WiFi.localIP()[3]);
  //ipAddr = WiFi.localIP();
  Serial.println(ipAddr);

  pinMode(ledPin, OUTPUT);

  if (chipId == "1495298")
  {
    yamahaTurnOn();
  }
  if (chipId == "313088")
  {
    //basementTurnOn();
  }

  pinMode(A0, INPUT); //water sensor
  //pinMode(A1, INPUT); //Analog Reading - ESP32 - VN pin

  DS18B20_Sensor.begin();
  getDS18B20Temp();
  
    if (BME680.begin())
   {
    bme680onBoard = true;
    // Set up oversampling and filter initialization
    BME680.setTemperatureOversampling(BME680_OS_8X);
    BME680.setHumidityOversampling(BME680_OS_2X);
    BME680.setPressureOversampling(BME680_OS_4X);
    BME680.setIIRFilterSize(BME680_FILTER_SIZE_3);
    BME680.setGasHeater(320, 150); // 320*C for 150 ms
    getBME680Data();
   }
 
  if (bme.begin())
  {
    bme280onBoard = true;
      switch(bme.chipModel())
      {
         case BME280::ChipModel_BME280:
           Serial.println("Found BME280 sensor! Success.");
           break;
         case BME280::ChipModel_BMP280:
           Serial.println("Found BMP280 sensor! No Humidity available.");
           break;
         default:
           Serial.println("Found UNKNOWN sensor! Error!");
      }

    getBME280Data();
  }
  else 
  {
    Serial.println("Did not find BME280 sensor! Fail.");
  }

  if (si7021.begin()) 
  {
    Serial.println("si7071 onBoard");
    si7021onBoard = true;
    si7021getReadings();
  }

  accelgyroIC1.initialize();
  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyroIC1.testConnection() ? "MPU6050-1 connection successful" : "MPU6050-1 connection failed");

  GetParamsFromWeb();
  const char* dnsName = SensorName.c_str();
  if (!MDNS.begin((dnsName))) {
    Serial.println("Error setting up MDNS responder!");
  }
  MDNS.addService("http", "tcp", 80);
  
  #if defined(ESP8266)
    Serial.printf("Default hostname: %s\n", WiFi.hostname().c_str());
    WiFi.hostname(dnsName);
    Serial.printf("New hostname: %s\n", WiFi.hostname().c_str());
  #else
    //Serial.printf("Default hostname: %s\n", WiFi.getHostname().c_str());
    WiFi.setHostname(dnsName);
    ////Serial.printf("New hostname: %s\n", WiFi.getHostname().c_str());
  #endif
  Serial.println("getAccelGyroData");
  getAccelGyroData();
  Serial.println("getAccelGyroData again");
  getAccelGyroData();//second call to set baseline

  Serial.println( WiFi.SSID() );
  Serial.println( WiFi.psk() );
  rssi = WiFi.RSSI();
  
  //analogVal = analogRead(A0);
  sendData("Startup", resetReason);


  //const char* updatePage = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";
  httpServer.on("/update", HTTP_GET, [](){
    httpServer.sendHeader("Connection", "close");
    httpServer.send(200, "text/html", "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>");
  });
  httpServer.on("/update", HTTP_POST, [](){
    httpServer.sendHeader("Connection", "close");
    httpServer.send(200, "text/plain", (Update.hasError())?"FAIL":"OK");
    ESP.restart();
  },[](){
    HTTPUpload& upload = httpServer.upload();
    if(upload.status == UPLOAD_FILE_START){
      Serial.setDebugOutput(true);
      //WiFiUDP::stopAll();
      Serial.printf("Update: %s\n", upload.filename.c_str());
      uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
      if(!Update.begin(maxSketchSpace)){//start with max available size
        Update.printError(Serial);
      }
    } else if(upload.status == UPLOAD_FILE_WRITE){
      if(Update.write(upload.buf, upload.currentSize) != upload.currentSize){
        Update.printError(Serial);
      }
    } else if(upload.status == UPLOAD_FILE_END){
      if(Update.end(true)){ //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
      Serial.setDebugOutput(false);
    }
    yield();
  });

  httpServer.on("/", handleWebsite);
  httpServer.on("/JSON", handleJSON);
  httpServer.on("/ledOn", []() {
    httpServer.send(200, "text/html", "Led On. <a href=javascript:history.go(-1)>Back</a> <a href=ledOff>Led Off</a>");
    digitalWrite(ledPin, HIGH); 
  });
  httpServer.on("/ledOff", []() {
    httpServer.send(200, "text/html", "Led Off. <a href=javascript:history.go(-1)>Back</a> <a href=ledOn>Led On</a>");
    digitalWrite(ledPin, LOW);
  });
  httpServer.on("/reboot", []() {
    httpServer.send(200, "text/html", "Reboot? <a href=yesReboot>Yes</a> <a href=javascript:history.go(-1)>No</a>");
  });
  httpServer.on("/yesReboot", []() {
    httpServer.send(200, "text/html", "<a href=javascript:history.go(-1)>Back</a>");
    writeEEPROM(10, "Web reboot button");
    ESP.restart();
    //delay(1000);
  });
  httpServer.on("/sendAlert", []() {
    String returnVal = sendData("Web button pushed", "true");
    httpServer.send(200, "text/html", "http://makesmart.io/addSend2.php?" + returnVal);
    //delay(1000);
  });

  httpServer.on("/resetWifi", []() {
    httpServer.send(200, "text/html", "Erase wifi config? <a href=yesResetWifi>Yes</a> <a href=javascript:history.go(-1)>No</a>");
  });
  httpServer.on("/yesResetWifi", []() {
    httpServer.send(200, "text/html", "Erasing wifi. <a href=/</a>");
    writeEEPROM(10, "Web resetWifi button");
    resetWifi();
    ESP.restart();
  });

  client.print("HTTP/1.1 301 Moved Permanently\r\nLocation: http://makeSmart.io/favicon.ico\r\n\r\n");

  httpServer.begin();
  Serial.printf("HTTP Server ready! Open http://%s.local/ in your browser\n", SensorName.c_str());

  ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  ads.begin();

}
