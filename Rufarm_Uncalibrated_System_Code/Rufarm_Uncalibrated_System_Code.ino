#include<Wire.h>
#include <ESP8266WiFi.h>
#include<ArduinoJson.h>
#include<Adafruit_ADS1015.h>

Adafruit_ADS1115 ads;

const float multiplier = 0.0001875F;

float pH_val;
float m;
float c;
float read_do_zero;
float read_do_air;
float do_volt;
float do_perct;
float abs_perct;


int i,n=3;

float stds[3] = {4 , 7 , 9.2};

void calibrate_pH(){
 float sig_x = 0 , sig_y = 0 , sig_xy = 0 , sig_x2 = 0;
 float volts[3];
 Serial.println("Place the probe in 4 pH solution...\n");
 delay(10000);
 Serial.println("Wait for voltage to stabilize...\n");
 delay(60000);
 volts[0] = ads.readADC_SingleEnded(0) * multiplier ;
 
 Serial.println("Place the probe in 7 pH solution...\n");
 delay(10000);
 Serial.println("Wait for voltage to stabilize...\n");
 delay(60000);
 volts[1] = ads.readADC_SingleEnded(0) * multiplier ;
 
 Serial.println("Place the probe in 9.2 pH solution...\n");
 delay(10000);
 Serial.println("Wait for voltage to stabilize...\n");
 delay(60000);
 volts[2] = ads.readADC_SingleEnded(0) * multiplier ;
 
  for(i=0;i<n;i++){
     sig_y = sig_y + volts[i];
     sig_x = sig_x + stds[i];
     sig_x2 = sig_x2 + (stds[i] * stds[i]);
     sig_xy = sig_xy + (stds[i] * volts[i]);
  }
  
  float m_num = (n * sig_xy) - (sig_x * sig_y);
  float denom = (n * sig_x2) - (sig_x * sig_x);
  m = m_num / denom;

  float c_num = (sig_x2 * sig_y) - (sig_x * sig_xy);
  float denom1 = (n * sig_x2) - (sig_x * sig_x);
  c = c_num / denom1;
  Serial.println("m ,c values : \t");
  Serial.print(m);
  Serial.print("\t\t");
  Serial.print(c);
  delay(2000);
  get_pH_val();
  
}

void get_pH_val(){
  Serial.println("\n\n");
  Serial.println("Place the probe in the solution...\n");
  delay(10000);
  Serial.println("Wait for voltage to stabilize...\n");
  delay(60000);
  float read_pH_volt = ads.readADC_SingleEnded(0) * multiplier ;
  float pH_val1 = read_pH_volt - c;
  pH_val = pH_val1 / m;
  Serial.print("The pH value is : \t");
  Serial.println(pH_val);
}

void calibrate_do(){
  Serial.println("Place the probe in the zero solution...\n");
  delay(10000);
  Serial.println("Wait for voltage to stabilize...\n");
  delay(60000);
  read_do_zero = ads.readADC_SingleEnded(1) * multiplier ;
  Serial.println("Place the probe in the air...\n");
  delay(10000);
  Serial.println("Wait for voltage to stabilize...\n");
  delay(60000);
  read_do_air = ads.readADC_SingleEnded(1) * multiplier ;
  delay(1000);
  get_do_val();
}

void get_do_val(){
  Serial.println("Place the probe in the pond...\n");
  delay(10000);
  Serial.println("Wait for voltage to stabilize...\n");
  delay(60000);
  do_volt = ads.readADC_SingleEnded(1) * multiplier;
  float volt_100 = read_do_air - read_do_zero;
  float volt_new = do_volt - read_do_zero ; 
  float do_num = volt_new * 100;
  do_perct = do_num / volt_100 ;
  abs_perct = abs(do_perct);
  Serial.print("The Do2 value is : ");
  Serial.println(abs_perct);
  delay(1000);
}

void setup() {
  Serial.begin(9600);
  ads.begin();
  ads.setGain(GAIN_TWOTHIRDS);
}



void loop() {
  calibrate_pH();
  calibrate_do();
}
