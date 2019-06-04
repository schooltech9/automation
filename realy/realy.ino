#include <ESP8266WiFi.h>
#include<ArduinoJson.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "ldr-value.firebaseio.com"
#define WIFI_SSID "Robic Rufarm"
#define WIFI_PASSWORD "Robicruf@rm12345"

int relayInput = 2; // the input to the relay pin


void setup() {
pinMode(relayInput, OUTPUT); // initialize pin as OUTPUT
Serial.begin(9600); // Starts the serial communication

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
  Firebase.setInt("LEDStatus",0); 
}

void loop() {
if(Firebase.getInt("LEDStatus"))
{
  digitalWrite(relayInput, HIGH); // turn relay on
}

else
{
  digitalWrite(relayInput, LOW); // turn relay off
}  
if (Firebase.failed()) {
    Serial.println("Firebase Pushing /sensor/failed:");
    Serial.println(Firebase.error()); 
    }
  else{
      Serial.print("Firebase Pushed /sensor/ ");
      }
delay(2000);
}
