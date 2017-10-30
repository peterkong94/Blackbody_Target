/*    userInputSetup
Description:	Initializes all of the pins needed for user input of the
				temperature
Input:			none
Output:			none
*/
void userInputSetup()
{
	pinMode(DECREASE_TEMP_PIN, INPUT);
	pinMode(INCREASE_TEMP_PIN, INPUT);
	pinMode(CHANGE_MODE_PIN, INPUT);
	pinMode(CHANGE_TARGET_PIN, INPUT);
}

/*    changeModes
Description:	Given the original mode (relative temp or absolute temp),
				we return the opposite temp mode.  
Input:			int orignal_mode -> the mode the controller is currently in
Output:			The other mode.  
*/
int changeModes(int original_mode)
{
	// if the mode is currently absolute temperature
	if (original_mode == ABS_TEMP)
	{
		// return relative temperature
		return REL_TEMP;
	}
	// if the mode is currently relative temp
	else if (original_mode == REL_TEMP)
	{
		// return absolute temperature
		return ABS_TEMP;
	}
	else // should not occur, but check 
	{
		return ERROR_MD;
	}
}

/*    changeTarget
Description:	Given the original target (heated or cooled),
                we return the other target.
Input:			int orignal_target -> the target currently being adjusted
Output:			The other target
*/
int changeTarget(int original_target)
{
	// if the target is currently the heated target
	if (original_target == HEATED_TARGET)
	{
		// return the cooled target
		return COOLED_TARGET;
	}
	// if the mode is currently the cooled target
	else if (original_target == COOLED_TARGET)
	{
		// return heated target
		return HEATED_TARGET;
	}
	else // should not occur, but check 
	{
		return ERROR_MD;
	}
}

/*    userInputLoop
Description:	Checks the user input and changes the temperature 
				accordingly. The setpoint temperature can be increased or
				decreased. Also the mode can be changed. 
Input:			int tempMode -> the mode the controller is currently in
				int relTemp  -> the current relative temperature setpoint 
				int absTemp  -> the current absolute temperature setpoint
Output:			All of the variables are passed by reference and therefore
				are being returned to the user changed.

*/
void userInputLoop(int& tempMode, int& relTemp, int& absTemp)
{
	// figure out which buttons are being pressed
	int tempIncrease = digitalRead(INCREASE_TEMP_PIN);
	int tempDecrease = digitalRead(DECREASE_TEMP_PIN);
	int modeButton = digitalRead(CHANGE_MODE_PIN);

	// the user wants to change the mode
	if (modeButton == PRESSED)
	{
	    //Serial.println("Temperature Mode has been changed!");
        tempMode = changeModes(tempMode);
	}
       
	// the current mode is relative temp
	if (tempMode == REL_TEMP)
	{
		// temperature needs to be increased
		if (tempIncrease == PRESSED && tempDecrease == NOT_PRESSED)
		{
			relTemp = relTemp + 1;
                        Serial.println("Increase temperature!");
		}
		// temperature needs to be decreased
		else if (tempIncrease == NOT_PRESSED && tempDecrease == PRESSED)
		{
			relTemp = relTemp - 1;
                        Serial.println("Decrease temperature!");
		}
	}
	// the current mode is absolute temp
	else if (tempMode == ABS_TEMP)
	{
		//temperature needs to be increased
		if (tempIncrease == PRESSED && tempDecrease == NOT_PRESSED)
		{
			Serial.println("Increase temperature!");
                        absTemp = absTemp + 1;
		}
		// temperature needs to be decreased
		else if (tempIncrease == NOT_PRESSED && tempDecrease == PRESSED)
		{
                         Serial.println("Decrease temperature!");
			absTemp = absTemp - 1;
		}
	}
	else
	{
		Serial.println("Error! Incorrect Temperature Mode");
	}



}

/*    setSetpoint
Description:	Checks the user input and changes the temperature
				accordingly. The setpoint temperature can be increased or
				decreased. Also the mode can be changed.
Input:			int setTemp    -> the setpoint
				int tempMode   -> the temperature mode
				int relTemp    -> the relative temperature setpoint
				int curAmbTemp -> the ambient temperature
				int absTemp    -> the absolute temperature setpoint
Output:			setTemp is passed as a reference and will be returned 
                possibly modified
*/
void setSetpoint(int& setTemp, int tempMode, int relTemp, int curAmbTemp, int absTemp)
{
	// if mode is absolute temperature
	// setpoint is the user inputted abs temp
	if (tempMode == ABS_TEMP)
	{
		setTemp = absTemp;
	}
	// if mode is relative temperature
	// setpoint is the user inputted relative temp
	else if (tempMode == REL_TEMP)
	{
		setTemp = curAmbTemp + relTemp;
	}
	else
	{
		Serial.println("Error! Incorrect Temperature Mode");
	}

}

/*   userInputLoopTargetChange
Description:	Given the original target (heated or cooled),
                we assign the other target.
Input:			int orignal_target -> the target currently being adjusted
Output:			nothing ( BUT a reference variable is changed)
*/
void userInputLoopTargetChange(int& original_target)
{
	// read user input
	int changeTargetButton = digitalRead(CHANGE_TARGET_PIN);

	// if the user wants to change the target to modify
	if (changeTargetButton == PRESSED) {
	
		// if the target is currently the heated target
		if (original_target == HEATED_TARGET)
		{
			// return the cooled target
			original_target = COOLED_TARGET;
		}
		// if the mode is currently the cooled target
		else if (original_target == COOLED_TARGET)
		{
			// return heated target
			original_target = HEATED_TARGET;
		}
		else // should not occur, but check 
		{
			original_target = ERROR_MD;
		}
	}
}