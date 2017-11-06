/* this is source code aims to get the temperature reading from the sensors
since we are currently using a temperautre chip this part of the programe is quite simple, 
However if we decide on using thermocouples this code could get more complex
*/

/*

//get the current temperature from the sensor 
double getCurrentTemp(int tempPin) {
  float voltage = 0, degreesC, degreesF;
  
  //do a averaging of 5 ilterations to inscrease accurecy 
  for (int i = 0; i < 5; i++){
    voltage += analogRead(tempPin)*0.005; //*0.005 to convert the 0 to 1034 analog read to 0 to 5 volts 
    delay(10);
  }
  
  voltage = voltage/5; 
  //this formula is obtained through experimental calibration, could get a more accurate formula 
  degreesC = 195 - voltage*100; 
  degreesF = degreesC * (9.0/5.0) + 32.0;
  
  //we only need the temperature in celcius as the control input 
  return degreesC; 

}
*/