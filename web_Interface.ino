void handleWebsite() {
  buildWebsite();
  httpServer.send(200, "text/html", webSite);
}

void handleJSON() {
  buildJson();
  httpServer.send(200, "text/json", JSON);
}
void buildWebsite() {

  buildJavascript();
  webSite = "<!DOCTYPE html>\n";
  webSite += "<title>" + SensorName + "</title>";
  webSite += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  webSite += "<link rel='shortcut icon' type='image/png' href='http://makeSmart.io/favicon.ico'/>";
  webSite += "<script src='//code.jquery.com/jquery-3.2.1.min.js'></script>";
  webSite += "<style>\n";
  webSite += ".button {\n";
  webSite += "  background-color: grey;\n";
  webSite += "  border: none;\n";
  webSite += "  color: white;\n";
  webSite += "  padding: 10px 14px;\n";
  webSite += "  text-align: center;\n";
  webSite += "  text-decoration: none;\n";
  webSite += "  display: inline-block;\n";
  webSite += "  font-size: 12px;\n";
  webSite += "  margin: 4px 2px;\n";
  webSite += "  cursor: pointer;}\n";
  webSite += "body {font-family: Sans-Serif;}";
  webSite += "</style>\n";
  webSite += javaScript;
  webSite += "<body>\n";
  webSite += "<h1>" + SensorName + "</h1> <h3>"+ipAddr+" on "+WiFi.SSID()+" RSSI: <a id='rssi'></a></h3>";
  webSite += "<p><a href=\"ledOn\"><button class='button'>Led On</button></a> <a href=\"ledOff\"> <button class='button'>Led Off</button></a>";
  webSite += " <a href=\"sendAlert\"><button class='button'>Send Alert Test</button></a>";
  webSite += " <a href=\"update\"><button class='button'>Update Firmware</button></a>";
  webSite += " <a href=\"resetWifi\"><button class='button'>Erase WIFI settings and reboot</button></a>";
  webSite += " <a href=\"reboot\"><button class='button'>Reboot</button></a></p>";
  webSite += "Runtime = <a id='upTime'></a>\n";
  webSite += "Accel1 = <a id='accel1'></a>\n";
  webSite += "Gyro1 = <a id='gyro1'></a>\n";
  webSite += "A0 = <a id='A0'></a>\n";
  {
    webSite += "bmeTemp = <a id='bmeTemp'></a>\n";
    webSite += "bmeHumi = <a id='bmeHumi'></a>\n";
    webSite += "bmePressure = <a id='bmePres'></a>\n";
  }
  if (sensorMode=="Washer")
  {
    webSite += "washerRunning = <a id='washerRunning'></a>\n";
    webSite += "washerStarting = <a id='washerStarting'></a>\n";
    webSite += "washerStopping = <a id='washerStopping'></a>\n";
  }
  if (sensorMode=="Dryer")
  {
    webSite += "dryerRunning = <a id='dryerRunning'></a>\n";
    webSite += "dryerStarting = <a id='dryerStarting'></a>\n";
    webSite += "dryerStopping = <a id='dryerStopping'></a>\n";
  }
  if (Probe1 > -196) {
    webSite += "Probe1 = <a id='Probe1'></a>\n";
  }
  if (Probe2 > -196) {
    webSite += "Probe2 = <a id='Probe2'></a>\n";
  }
  webSite += "Access Point MAC = <a id='apMac'></a>\n";
  webSite += "<p>View data online: <a href=https://makesmart.io/data.php?pageType=Detail&RockID=" + chipId + ">https://makesmart.io/data.php?pageType=Detail&RockID=" + chipId + "</a>";
  webSite += "<iframe width='100%' height='500' frameborder='1' src='https://makeSmart.io/data.php?pageType=Detail&RockID=" + chipId + "'></iframe>\n";

  webSite += "</body>\n";
  webSite += "</html>\n";
}

void buildJavascript() {
  javaScript = "<script>\n ";
  javaScript += "var myVar = setInterval(pollData, 5000);\n";
  javaScript += "function pollData() { \n";
  javaScript += " console.log('Polling');\n";
  javaScript += "  $.ajax({\n";
  javaScript += "      url: '/JSON',\n";
  javaScript += "      type: 'GET',\n";
  javaScript += "      success: function(data) {\n";
  javaScript += "        console.log(data);\n";

  javaScript += " $('#upTime').text(data.upTime); \n";
  javaScript += " $('#rssi').text(data.rssi); \n";
  javaScript += " $('#bmeTemp').text(data.bmeTemp); \n";
  javaScript += " $('#bmeHumi').text(data.bmeHumi); \n";
  javaScript += " $('#bmePres').text(data.bmePres); \n";
  javaScript += " $('#A0').text(data.A0); \n";
  javaScript += " $('#accel1').text(data.accel1); \n";
  javaScript += " $('#gyro1').text(data.gyro1); \n";
  javaScript += " $('#Probe1').text(data.Probe1); \n";
  javaScript += " $('#Probe2').text(data.Probe2); \n";
  javaScript += " $('#apMac').text(data.apMac); \n";

  if (sensorMode=="Washer")
  {
  javaScript += " $('#washerRunning').text(data.washerRunning); \n";
  javaScript += " $('#washerStarting').text(data.washerStarting); \n";
  javaScript += " $('#washerStopping').text(data.washerStopping); \n";
  }
  
  if (sensorMode=="Dryer")
  {
  javaScript += " $('#dryerRunning').text(data.dryerRunning); \n";
  javaScript += " $('#dryerStarting').text(data.dryerStarting); \n";
  javaScript += " $('#dryerStopping').text(data.dryerStopping); \n";
  }

  javaScript += "      },\n";
  javaScript += "      error: function (request, status, error) {\n";
  javaScript += "        console.log(request.responseText);\n";
  javaScript += "        console.log(error);\n";
  javaScript += "      },\n";
  javaScript += "      timeout: 2000\n";
  javaScript += "  });\n";
  javaScript += "}\n";
  
  javaScript += "</script>\n";
}

void buildJson() {
  //JSON = "{""\"name\"": "\"Test_Temp\"", "\"service_name\"";
  //return;
  //JSON = "{"pin36": 5.2, "pin39": 0.322, "pin5": 1}";
  JSON = "{\"upTime\":";
  JSON += "\"" + millis2time() + "\"";
  JSON += ",\"bmeTemp\":";
  JSON += bmeTemp;
  JSON += ",\"bmeHumi\":";
  JSON += bmeHumi;
  JSON += ",\"bmePres\":";
  JSON += bmePres;
  JSON += ",\"accel1\":";
  JSON += accel1;
  JSON += ",\"gyro1\":";
  JSON += gyro1;
  JSON += ",\"rssi\":";
  JSON += rssi;
  JSON += ",\"A0\":";
  JSON += analogVal;

  JSON += ",\"Probe1\":";
  JSON += Probe1;
  JSON += ",\"Probe2\":";
  JSON += Probe2;
  
  JSON += ",\"accel1\":";
  JSON += accel1;
  JSON += ",\"gyro1\":";
  JSON += gyro1;
  
  JSON += ",\"washerRunning\":";
  JSON += washerRunning;
  JSON += ",\"washerStarting\":";
  JSON += washerStarting;
  JSON += ",\"washerStopping\":";
  JSON += washerStopping;
  
  JSON += ",\"dryerRunning\":";
  JSON += dryerRunning;
  JSON += ",\"dryerStarting\":";
  JSON += dryerStarting;
  JSON += ",\"dryerStopping\":";
  JSON += dryerStopping;

  JSON += ",\"apMac\":";
  JSON += "\"" + WiFi.BSSIDstr() + "\"";

  JSON += "}";
  return;
}
String millis2time() {
  String Time = "";
  unsigned long ss;
  byte mm, hh;
  ss = millis() / 1000;
  hh = ss / 3600;
  mm = (ss - hh * 3600) / 60;
  ss = (ss - hh * 3600) - mm * 60;
  if (hh < 10)Time += "0";
  Time += (String)hh + ":";
  if (mm < 10)Time += "0";
  Time += (String)mm + ":";
  if (ss < 10)Time += "0";
  Time += (String)ss;
  return Time;
}
