/*    LCDSetup
Description:	Initializes the LCD and prints a message
Input:			none
Output:			none
*/
void LCDSetup()
{
	// initialize the lcd  
	lcd.init();           
	// turn on the backlight
	lcd.backlight();
	// Print a message to the LCD.
	lcd.print("current temp: ");
	// set up the LCD's number of columns and rows:
	lcd.setCursor(0, 2);
	// Print a message to the LCD.
	lcd.print("set temp: ");
}


/*    dispCurrentTemp
Description:	Display to the current temp field of the LCD
Input:			currentTemp -> the current temperature that
					           is outputted to the LCD
Output:			none
*/
void dispCurrentTemp(double currentTemp)
{
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(currentTemp);
}

/*    dispSetTemp
Description:	Display to the setpoint field of the LCD 
Input:			setTemp -> the setpoint that will outputted to the
				           LCD
Output:			none
*/
void dispSetTemp(double setTemp)
{
  lcd.setCursor(0, 3);
  lcd.print(setTemp);
}

/*    dispError
Description:	Display to the error field of the LCD
Input:			error -> value displayed to LCD
Output:			none
*/
void dispError(double error)
{
  //this is to clear the previous printed value to prevent overlap
  lcd.setCursor(13, 3);
  lcd.print("     ");
  lcd.setCursor(7, 1);
  lcd.print("Err: ");
  lcd.setCursor(13, 1);
  lcd.print(error);
}

//the PWM is the control signal to the TE-COOLER driver
/*    dispPWM
Description:	PWM is the control signal to the TE-Cooler driver.
				Display a value to the PWM field of the LCD
Input:			controlS -> value displayed to LCD
Output:			none
*/
void disPWM(int controlS)
{
    lcd.setCursor(7, 3);
    lcd.print("PMW: ");
    lcd.setCursor(13, 3);
    lcd.print(controlS);
  /*if(controlS > 0){ 
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
  }*/
}
