//This is the thermocouple chip initilaztion roution 
//It should only be called once in set up 
void thermoInit() {
  Serial.println("MAX6675 test");
  // wait for MAX chip to stabilize
  delay(500);
}
