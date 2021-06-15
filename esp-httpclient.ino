#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#define SerialEsp Serial
ESP8266WiFiMulti WiFiMulti;
String bufClient;
String sensor;
String nilai;

void sendData(String data){
  data.trim();
  //post("NamaSensor","Nilai")
  //post("Soil","99")
  int len = data.length();
  int sdata = data.indexOf('(');
  int edata = data.indexOf(')');
  int mdata = data.indexOf(',');
  sensor = data.substring(sdata + 2, mdata - 1);    
  nilai = data.substring(mdata + 2, edata - 1);
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    HTTPClient http;
    http.begin("http://your-server/file.php?sensor=" + sensor + "&nilai=" + nilai); 
    int httpCode = http.GET();
    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        SerialEsp.println(payload);
      }
    } else {
      SerialEsp.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
  bufClient = "";    
  sensor = "";
  nilai = "";
}

void setup() {
  SerialEsp.begin(115200);
  delay(4000);
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("WiFi Tetangga", "*********");
}

void loop() {
  if (Serial.available()) {
    bufClient = "";
    while (Serial.available()) {
      bufClient += Serial.readString();       
    }
  }
  if (bufClient > ""){    
    sendData(bufClient);
  }
}