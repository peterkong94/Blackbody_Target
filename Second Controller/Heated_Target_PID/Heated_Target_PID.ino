#include <PID_v1.h>
#include "max6675.h"

using namespace std;

const int PREDEFINED_TEMP_SETPOINT_HEATED = 23;

// setup for thermoCouple Pins Amps 
// thermocouple pins for blackbody temperature
const int THERMO_DO_BB_PIN_HEATED = 4;
const int THERMO_CS_BB_PIN_HEATED = 5;
const int THERMO_CLK_BB_PIN_HEATED = 6;

// tuning variables for the PID loop
const int TUNING_P_HEATED = 8;
const int TUNING_I_HEATED = 15;
const int TUNING_D_HEATED = 3;

// PIN Allocation
const int HEAT_PIN = 9;

// Global Variables
//Define PID Variables
double setpoint_heat, input_heat, output_heat;

// Thermocouple Objects
MAX6675 thermocouple_blackbody_heated(THERMO_CLK_BB_PIN_HEATED, THERMO_CS_BB_PIN_HEATED, THERMO_DO_BB_PIN_HEATED);

PID PIDHeatedTarget(&input_heat, &output_heat, &setpoint_heat, TUNING_P_HEATED, TUNING_I_HEATED, TUNING_D_HEATED, DIRECT);

double current_temp;

void setup() {

  // put your setup code here, to run once:
	pinMode(HEAT_PIN, OUTPUT);

	// set the input and setpoint of the PID loop
	pidSetup();

	// initialize the thermocouples
	thermoInit();

	Serial.begin(9600);
	Serial.setTimeout(100);
}

void loop() {
  // put your main code here, to run repeatedly:

	current_temp = thermocouple_blackbody_heated.readCelsius();
	float temp;
		
	// send current temp to main controller
	Serial.println(current_temp);
	//delay(1000);

	int iterations = 0;
	// retrieve the setpoint from the main controller
	// saved the latest setpoint into our setpoint

	while (Serial.available() && iterations <= 100)
	{
		// read 8 bytes for a double
		temp = Serial.parseInt();
		if (int(temp) != 0) 
		{
			setpoint_heat = temp;
		}
	}

	// calculate the PID with the current temp and setpoint
	pidCalc(current_temp);

	// send a signal out to the heater to power it on and off
	tempCon();

}

