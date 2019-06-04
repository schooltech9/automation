#include <ESP8266WiFi.h>
#include<ArduinoJson.h>
#include <FirebaseArduino.h>
#include <SoftwareSerial.h>
char character;

#define FIREBASE_HOST "ldr-value.firebaseio.com"
#define WIFI_SSID "Robic Rufarm"
#define WIFI_PASSWORD "Robicruf@rm12345"


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST);
}

void loop() { // run over and over
      String Data = "";

  if (Serial.available()) {
    int X = Serial.read();
    char y = Serial.read();
    Serial.print("Char:");
    Serial.println(y);
    String str = String(X);
    float f = float(X);
    String xval = getValue(str, ':', 0);
    String yval = getValue(str, ':', 1);
    
    character = Serial.read();
    Data.concat(character);
    Serial.println(Data);
    
    Serial.print("f data:");
    Serial.println(f);
    
    Serial.print("incomming data:");
    Serial.println(X);

    Serial.print("Y:" + yval);
    Serial.println("X:" + xval);

    int inByte = Serial.read();
    Serial.print("inbyte:");
    Serial.println(inByte,DEC);
    delay(73000);
  }
}
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
