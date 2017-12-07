/*    pidSetup
Description:	Set the input and setpoint for the PID loop.
Turn the PID on.
Input:			thermocouple object to read the input from
Output:		none
*/
void pidSetup()
{
	// set the input to the thermocouple inputted and setpoint to 0
	input_heat = averageTempCalc();
	
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

/*    averageTempCalc    
Description: This function calculates the average temperature of the 
target and returns that value. It also places each thermocouple 
temperature into its own variable, either target_temp1, target_temp2,
target_temp3, or target_temp4.
Input:		none
Output:		the average temperature of the target based on the four 
			target temperature measurements.
*/
double averageTempCalc()
{
	// get all 4 temperature measurements and store them
	target_temp1 = Thermo_1.readThermocoupleTemperature();
	target_temp2 = Thermo_2.readThermocoupleTemperature();
	target_temp3 = Thermo_3.readThermocoupleTemperature();
	target_temp4 = Thermo_4.readThermocoupleTemperature();

	// return the average of all four measurements
	return (target_temp1 + target_temp2 + target_temp3 + target_temp4) / 4;
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
	
	digitalWrite(RELAY_CONTROL, HIGH);
	delay(OnTimeHeat);
	if (OnTimeHeat != 255) {
		digitalWrite(RELAY_CONTROL, LOW);
		delay(255 - OnTimeHeat);
	}

}
