 /*    pidSetup
 Description:	Set the input and setpoint for the PID loop.
				Turn the PID on. 
 Input:			thermocouple object to read the input from 
 Output:		none
 */
void pidSetup()
{
	// set the input to the thermocouple inputted and setpoint to 0
	input_cool = thermocouple_blackbody_cooled.readCelsius();
	setpoint_cool = PREDEFINED_TEMP_SETPOINT_COOLED;

	//turn the PID on
	PIDCooledTarget.SetMode(AUTOMATIC);

	// start serial
	Serial.begin(9600);
}

/*    pidCalc
Description:	Calculate the PID to determine how much current
				to send to the heating and cooling elements
Input:			currentTemp -> the current temperature of the blackbody
				setTemp     -> the set temperature from the user
Output:			none
*/
void pidCalc()
{
	input_cool = blackbodies[COOLED_TARGET].currentTemp;
	setpoint_cool = blackbodies[COOLED_TARGET].setTemp;

  // output variable will be recomputed  
	PIDCooledTarget.Compute();
	
}

/*    tempCon
Description:	Create a large scale PWM given the output of the PID.
				For example, if the output is 133, then the pin will be
				high for 133ms and low for 122ms. Overall, the heat pin
				will be high for output time. Then it will be low for 
				255 - output time. 
Input:			none			 
Output:			none
*/
void tempCon()
{

	//int OnTimeHeat = output_heat; 
	int OnTimeCool = output_cool;

	digitalWrite(COOL_PIN, HIGH);
	delay(OnTimeCool);
	if (OnTimeCool != 255) {
		digitalWrite(COOL_PIN, LOW);
		delay(255 - OnTimeCool);
	}
}



