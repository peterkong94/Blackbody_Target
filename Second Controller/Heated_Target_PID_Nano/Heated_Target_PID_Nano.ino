#include <SPI.h>
#include <PID_v1.h>
#include "Adafruit_MAX31856.h"

using namespace std;

const int PREDEFINED_TEMP_SETPOINT_HEATED = 23;

// setup for thermoCouple Pins Amps 
// thermocouple pins for blackbody temperature

const int THERMO_GROUP1_SCK = 12;
const int THERMO_GROUP1_SDO = 11;
const int THERMO_GROUP1_SDI = 10;
const int THERMO1_ICS = 9;
const int THERMO2_ICS = 7;
const int THERMO3_ICS = 5;
const int THERMO4_ICS = 3;

// tuning variables for the PID loop
const int TUNING_P_HEATED = 40;
const int TUNING_I_HEATED = 40;
const int TUNING_D_HEATED = 0;

// Relay output
const int RELAY_CONTROL = 13;

// Global Variables
//Define PID Variables
double setpoint_heat, input_heat, output_heat;

// Thermocouple Objects
Adafruit_MAX31856 Thermo_1 = Adafruit_MAX31856(THERMO1_ICS, THERMO_GROUP1_SDI, THERMO_GROUP1_SDO, THERMO_GROUP1_SCK);
Adafruit_MAX31856 Thermo_2 = Adafruit_MAX31856(THERMO2_ICS, THERMO_GROUP1_SDI, THERMO_GROUP1_SDO, THERMO_GROUP1_SCK);
Adafruit_MAX31856 Thermo_3 = Adafruit_MAX31856(THERMO3_ICS, THERMO_GROUP1_SDI, THERMO_GROUP1_SDO, THERMO_GROUP1_SCK);
Adafruit_MAX31856 Thermo_4 = Adafruit_MAX31856(THERMO4_ICS, THERMO_GROUP1_SDI, THERMO_GROUP1_SDO, THERMO_GROUP1_SCK);

PID PIDHeatedTarget(&input_heat, &output_heat, &setpoint_heat, TUNING_P_HEATED, TUNING_I_HEATED, TUNING_D_HEATED, DIRECT);

double target_temp1;
double target_temp2;
double target_temp3;
double target_temp4;
double average_target_temp;

void setup() {

  // put your setup code here, to run once:
	pinMode(RELAY_CONTROL, OUTPUT);

	// initialize all 4 thermocouples
	boolean success1 = Thermo_1.begin();
	boolean success2 = Thermo_2.begin();
	boolean success3 = Thermo_3.begin();
	boolean success4 = Thermo_4.begin();
	
	// set the thermocouple types
	Thermo_1.setThermocoupleType(MAX31856_TCTYPE_T);
	Thermo_2.setThermocoupleType(MAX31856_TCTYPE_T);
	Thermo_3.setThermocoupleType(MAX31856_TCTYPE_T);
	Thermo_4.setThermocoupleType(MAX31856_TCTYPE_T);

	// set the input and setpoint of the PID loop
	pidSetup();

	// initialize the thermocouples
	// thermoInit();

	Serial.begin(9600);
	Serial.setTimeout(100);
}

void loop() {
  // put your main code here, to run repeatedly:

	average_target_temp = averageTempCalc();
	float temp;
		
	// send current temp to main controller
	//Serial.println(average_target_temp);
        Serial.println(target_temp1);
        Serial.println(target_temp2);
        Serial.println(target_temp3);
        Serial.println(target_temp4);

	int iterations = 0;
	// retrieve the setpoint from the main controller
	// saved the latest setpoint into our setpoint

	// added for testing purposes
		
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
	pidCalc(average_target_temp);

	// send a signal out to the heater to power it on and off
	tempCon();

}

