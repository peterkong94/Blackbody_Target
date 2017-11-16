/*    thermocouple_pin_setup()
Description:	Sets the pin mode for the thermocouples
Input:			currentTemp -> the current temperature of the blackbody
setTemp     -> the set temperature from the user
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
}