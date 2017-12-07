/*     setIndicators 
Description:	Turn on the indicator(s) if the target(s)
				are within .5 degrees Celsius
Input:			none
Output:			none
*/
void setIndicators()
{
	// if the heated targets temp is between +/- .5 degrees Celsius of the set temp
	// turn the light on
	if (blackbodies[HEATED_TARGET].currentTemp <= blackbodies[HEATED_TARGET].setTemp + .5 && blackbodies[HEATED_TARGET].currentTemp >= blackbodies[HEATED_TARGET].setTemp - .5)
	{
		digitalWrite(HEATED_LIGHT, HIGH);
	}
	else  // turn the light off
	{
		digitalWrite(HEATED_LIGHT, LOW);
	}

	// if the cooled targets temp is between +/- .5 degrees Celsius of the set temp
	// turn the light on
	if (blackbodies[COOLED_TARGET].currentTemp <= blackbodies[COOLED_TARGET].setTemp + .5 && blackbodies[COOLED_TARGET].currentTemp >= blackbodies[COOLED_TARGET].setTemp - .5)
	{
		digitalWrite(COOLED_LIGHT, HIGH);
	}
	else  // turn the light off
	{
		digitalWrite(COOLED_LIGHT, LOW);
	}
}
