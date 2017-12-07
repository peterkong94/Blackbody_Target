/*    pidSetup
 Description:	Set the input and setpoint for the PID loop.
				Turn the PID on. 
 Input:			thermocouple object to read the input from 
 Output:		none
 */
void pidSetup()
{
	// set the input to the thermocouple inputted and setpoint to 0
	input_cool = Thermo_1.readThermocoupleTemperature();
	setpoint_cool = PREDEFINED_TEMP_SETPOINT_COOLED;

	//turn the PID on
	PIDCooledTarget.SetMode(AUTOMATIC);

	// set the direction of the pid
	setPIDDirection();

	// start serial
	Serial.begin(9600);
}

/*    te_cooler_pin_setup()
Description:	Sets the pin mode for the te coolers
Input:			none
Output:			none
*/
void te_cooler_pin_setup()
{
	pinMode(T1_PWM, OUTPUT);
	pinMode(T2_PWM, OUTPUT);
	pinMode(T3_PWM, OUTPUT);
	pinMode(T4_PWM, OUTPUT);
	pinMode(T5_PWM, OUTPUT);
	pinMode(T6_PWM, OUTPUT);
	pinMode(T7_PWM, OUTPUT);
	pinMode(T8_PWM, OUTPUT);
	pinMode(T9_PWM, OUTPUT);

	// set the mode for the direction pins
	pinMode(T1_DIRECTION, OUTPUT);
	pinMode(T1_2_DIRECTION, OUTPUT);
	pinMode(T3_4_DIRECTION, OUTPUT);
	pinMode(T5_6_DIRECTION, OUTPUT);
	pinMode(T7_8_DIRECTION, OUTPUT);
	pinMode(T9_DIRECTION, OUTPUT);
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
 
	// output pwm to the controller
	analogWrite(T1_PWM, output_cool);
	analogWrite(T2_PWM, output_cool);
	analogWrite(T3_PWM, output_cool);
	analogWrite(T4_PWM, output_cool);
	analogWrite(T5_PWM, output_cool);
	analogWrite(T6_PWM, output_cool);
	analogWrite(T7_PWM, output_cool);
	analogWrite(T8_PWM, output_cool);
	analogWrite(T9_PWM, output_cool);

	/*
	int OnTimeCool = output_cool;

	digitalWrite(T1_PWM, HIGH);
	delay(OnTimeCool);
	if (OnTimeCool != 255) {
		digitalWrite(T1_PWM, LOW);
		delay(255 - OnTimeCool);
	}
	*/
}

/*  setPIDDirection
Description:	According to the setpoint and the ambient temperature set the
				direction of the PID.
Input:			none
Output:			none
*/
void setPIDDirection()
{
	// if the ambient temperature is high than the set temp
	// the target needs to be cooled
	if (blackbodies[COOLED_TARGET].setTemp < currentAmbientTemp)
	{
		// pid will calculate in order to cool
		PIDCooledTarget.SetControllerDirection(REVERSE);

		// te coolers are set to cool
		digitalWrite(T1_DIRECTION, COOL_TE_COOLERS);
		digitalWrite(T1_2_DIRECTION, COOL_TE_COOLERS);
		digitalWrite(T3_4_DIRECTION, COOL_TE_COOLERS);
		digitalWrite(T5_6_DIRECTION, COOL_TE_COOLERS);
		digitalWrite(T7_8_DIRECTION, COOL_TE_COOLERS);
		digitalWrite(T9_DIRECTION, COOL_TE_COOLERS);

	}
	else  // the target needs to be heated
	{
		// pid will calculate in order to heat the target
		PIDCooledTarget.SetControllerDirection(DIRECT);

		// te coolers direction set to heat
		digitalWrite(T1_DIRECTION, HEAT_TE_COOLERS);
		digitalWrite(T1_2_DIRECTION, HEAT_TE_COOLERS);
		digitalWrite(T3_4_DIRECTION, HEAT_TE_COOLERS);
		digitalWrite(T5_6_DIRECTION, HEAT_TE_COOLERS);
		digitalWrite(T7_8_DIRECTION, HEAT_TE_COOLERS);
		digitalWrite(T9_DIRECTION, HEAT_TE_COOLERS);
	}
}



