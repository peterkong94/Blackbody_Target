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
	lcd.print("Target: ");
	// set up the LCD's number of columns and rows:
	lcd.setCursor(0, 1);
	// Print a message to the LCD.
	lcd.print("Set Pt: ");
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
  lcd.setCursor(10, 0);
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
  lcd.setCursor(10, 1);
  lcd.print(setTemp);
}

/*    dispRelativity
Description:	Display to the temperature mode 
Input:			none
Output:			none
*/
void dispRelativity()
{
  //this is to clear the previous printed value to prevent overlap
  lcd.setCursor(17, 0);
  
  if (blackbodies[currentTarget].targetTempMode  == REL_TEMP)
  {
	  lcd.print("REL");
  }
  else if (blackbodies[currentTarget].targetTempMode == ABS_TEMP)
  {
	  lcd.print("ABS");
  }
  else
  {
	  lcd.print("ERR");
  }
}

void dispTarget()
{
	lcd.setCursor(7, 0);
	if (currentTarget == HEATED_TARGET)
	{
		lcd.print("H");
	}
	else
	{
		lcd.print("C");
	}	
}

void dispPlus_Minus()
{
	lcd.setCursor(0, 3);
	if (blackbodies[currentTarget].targetTempMode == REL_TEMP)
	{
		lcd.print("+/-:");
		lcd.print("   ");
		lcd.setCursor(4, 3);
		lcd.print(blackbodies[currentTarget].relativeTemp);
	}
	else
	{
		lcd.print("        ");
	}
}

void dispAmbTemp()
{
	lcd.setCursor(0, 2);
	lcd.print("Amb Temp:");
	lcd.setCursor(10, 2);
	lcd.print(currentAmbientTemp);

}

void dispHumidity()
{
	lcd.setCursor(11, 3);
	lcd.print("Hum:");
	lcd.print(relative_humidity);

}


