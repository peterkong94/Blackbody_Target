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
	input_cool = thermocouple_blackbody_cooled.readCelsius();
	setpoint_heat = 0;
	setpoint_cool = 0;

	//turn the PID on
	PIDHeatedTarget.SetMode(AUTOMATIC);
	PIDCooledTarget.SetMode(AUTOMATIC);
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
	input_heat = blackbodies[HEATED_TARGET].currentTemp;
	setpoint_heat = blackbodies[HEATED_TARGET].setTemp;
	input_cool = blackbodies[COOLED_TARGET].currentTemp;
	setpoint_cool = blackbodies[COOLED_TARGET].setTemp;

  // output variable will be recomputed  
	PIDCooledTarget.Compute();
	PIDHeatedTarget.Compute();
  //analogWrite(3,Output);

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
	int OnTimeCool = output_cool;

	digitalWrite(HEAT_PIN, HIGH); 
	delay(OnTimeHeat); 
	if (OnTimeHeat != 255) {
		digitalWrite(HEAT_PIN, LOW);
		delay(255 - OnTimeHeat);
	}
	
	digitalWrite(COOL_PIN, HIGH);
	delay(OnTimeCool);
	if (OnTimeCool != 255) {
		digitalWrite(COOL_PIN, LOW);
		delay(255 - OnTimeCool);
	}
}

/* this source file include methods for the Temp control algritham
two functions are currently in here:
1. simple porportional control: used for now for fast result, should be discarded once we figure out the PID algrithm 
2. PID control: our ultimate goal, however needs more research and testin
*/

/*this is our simple porportional control methode before we figure out the PID stuffs 
void pControl(int error){
  //6 is the proportional mutiplayer that I set, we can play around with it 
  int controlS = error*6;
  if(controlS > 0){ 
    digitalWrite(heatPin, LOW); 
    analogWrite(coolPin, controlS); 
  }else{ 
    digitalWrite(coolPin, LOW); 
    analogWrite(heatPin, controlS); 
  }
}
*/

