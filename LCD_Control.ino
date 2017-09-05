void LCDSetup(){
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  // Print a message to the LCD.
  lcd.print("current temp: ");
   // set up the LCD's number of columns and rows:
  lcd.setCursor(0, 2);
  // Print a message to the LCD.
  lcd.print("set temp: ");
}

void dispCurrentTemp(double currentTemp){
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(currentTemp);
}

void dispSetTemp(double setTemp){
  lcd.setCursor(0, 3);
  lcd.print(setTemp);
}

void dispError(double error){
  //this is to clear the previous printed value to prevent overlap
  lcd.setCursor(13, 3);
  lcd.print("     ");
  
  lcd.setCursor(7, 1);
  lcd.print("Err: ");
  lcd.setCursor(13, 1);
  lcd.print(error);
}

//the PWM is the control signal to the TE-COOLER driver
void disPWM(int controlS){
  if(controlS > 0){ 
    lcd.setCursor(5, 3);
    lcd.print("PMW: -");
    lcd.setCursor(11, 3);
    lcd.print(controlS);
  }else{ 
    controlS *= -1;
    lcd.setCursor(5, 3);
    lcd.print("PMW: +");
    lcd.setCursor(11, 3);
    lcd.print(controlS);
  }
}
