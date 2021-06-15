#include <SoftwareSerial.h>

SoftwareSerial espSerial(12, 10);	//pin serial ke esp
String bufClient;
int relay = 5;
unsigned long previousMillis = 0;
const long interval = 60000;    	//nilai waktu (60 detik) untuk trigger lakukan sesuatu

void setup() {
  Serial.begin(115200);
  espSerial.begin(115200);
  pinMode(relay, OUTPUT);
}

void loop() {
  int Signal = analogRead(A1);
  unsigned long currentMillis = millis();		//get current millisecond
  if (espSerial.available()) {					//baca serial esp
    bufClient = "";
    while (espSerial.available()) {
      bufClient += espSerial.readString();      //kumpulin jadi satu
    }
  }
  if (bufClient > ""){    						//eksekusi
    Serial.println(bufClient);
	if (bufClient == "RelayON"){
		digitalWrite(relay, HIGH);
	}
	else if (bufClient == "RelayOFF"){
		digitalWrite(relay, LOW);
	}
    bufClient = "";
  }
  if ((currentMillis - previousMillis) >= interval) {	//jalanin sesuatu setiap 1 menit.
	  //post("NamaSensor","Nilai")
	  //post("Soil","99")
      String tipe = "post";
      String sensor = "Soil";
      String nilai = String(Signal);
      String f = tipe + "('" + sensor + "','" + nilai + "')";
      espSerial.println(f);
	  Serial.println("Sending value to ESP");
	  previousMillis = currentMillis;         			//set timer ke nilai terbaru
	  
  }
  //Serial.println(Signal);
  delay(15);
}
