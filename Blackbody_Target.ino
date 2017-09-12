//this is the main source file 
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int heatPin = 9;           // the PWM pin the LED is attached to
int coolPin = 10;    // how bright the LED is
#define TEMPUP_PIN               6  // temp up
#define TEMPDOWN_PIN             7  // temp down 

#include <math.h>
const int temperaturePin = 2;
const int setTempPin = 3;
int tempTemp = 0;
int setTemp = 15; 
double currentTemp = 0; 
double prevTemp = 0; 
void setup() {
  // put your setup code here, to run once
  //setup LCD display
  LCDSetup();
  //set TE-COOLER output pin 
  pinMode(heatPin, OUTPUT);
  pinMode(coolPin, OUTPUT);
  //pinMode(temperaturePin, INPUT);
  //pinMode(setTempPin, INPUT);
  prevTemp = getCurrentTemp();
  // event init
  eventInit();
  Serial.begin(9600);
}

void loop() {
  //handle button
  eventHandle();

  //Serial.print(digitalRead(TEMPUP_PIN));
  //Serial.print("/");
  dispSetTemp(setTemp);
  // put your main code here, to run repeatedly:
  currentTemp = (getCurrentTemp() + prevTemp)/2;
  dispCurrentTemp(currentTemp);
  
  int error = currentTemp - setTemp;
  dispError(error); 
  pControl(error); 
  
  //for averagying from last loop to reduce spikes, could be elimiated 
  prevTemp = currentTemp; 
  delay(500); 
  digitalWrite(heatPin, LOW); 
  digitalWrite(coolPin, LOW);


}
