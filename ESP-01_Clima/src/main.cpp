#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "settings.h"
#include "Wire.h"
#include "BMP280.h"


#define delay_out 60000  //delay between eatch DB entry

String SERIAL_TAG = "S001";

HTTPClient http;

void push_to_db(String, double);
BMP280 bmp;

void setup() {
  Wire.begin(0,2);
  Serial.begin(9600);
  Serial.setTimeout(2000);

  Serial.println("Device Started");

  if(!bmp.begin()){
    Serial.println("BMP init failed!");
    while(1);
  }

  WiFi.mode(WIFI_STA);
  WiFi.setOutputPower(0.0);
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Not connected");
    delay(100);
  }

  Serial.println("WiFi connected");

}

void loop() {
  double T,P;
  bmp.startMeasurment();
  bmp.getTemperatureAndPressure(T,P);
  push_to_db("Temp", T);
  push_to_db("Pres", P);
  delay(delay_out);
}


//void push_to_db(char *tag, char *series, double adc)
void push_to_db(String tag, double adc)
{
  /*
  char buffer[256];
  http.begin(INFLUXDB_URI);
  sprintf(buffer, "%s,sensor=%s value=%f", series, tag, adc );
  http.POST(buffer);
  http.end();
  Serial.printf("%s,\tSensor = %s\tvalue = %f\n", series, tag, adc);
  */
http.begin("http://MF-Technologie.de:8086/write?db=fehren_DB");

http.addHeader("Content-Type", "--data-binary");  //Specify content-type header, I have figured out that it   can be left out

String postData ="clima,serialNr=" + SERIAL_TAG + ",tag=" + tag + " value=" + adc;

int httpCode = http.POST(postData);

String text = tag + " = " + adc;

Serial.println(text);

http.end();  //Close connection
}
