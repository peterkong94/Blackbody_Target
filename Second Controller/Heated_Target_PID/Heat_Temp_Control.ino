/*    pidSetup
Description:	Set the input and setpoint for the PID loop.
Turn the PID on.
Input:			thermocouple object to read the input from
Output:		none
*/
void pidSetup()
{
	// set the input to the thermocouple inputted and setpoint to 0
	input_heat = thermocouple_blackbody_heated.readCelsius();
	
	setpoint_heat = PREDEFINED_TEMP_SETPOINT_HEATED;

	//turn the PID on
	PIDHeatedTarget.SetMode(AUTOMATIC);

}

/*    pidCalc
Description:	Calculate the PID to determine how much current
to send to the heating and cooling elements
Input:			currentTemp -> the current temperature of the blackbody
setTemp     -> the set temperature from the user
Output:			none
*/
void pidCalc(double currentTemp)
{
	input_heat = currentTemp;

	// output variable will be recomputed  
	PIDHeatedTarget.Compute();
	
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

	int OnTimeHeat = output_heat;
	
	digitalWrite(HEAT_PIN, HIGH);
	delay(OnTimeHeat);
	if (OnTimeHeat != 255) {
		digitalWrite(HEAT_PIN, LOW);
		delay(255 - OnTimeHeat);
	}

}