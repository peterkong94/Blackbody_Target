// 8 thermocouples max31856 .h and .cpp (Google MAX31856) for thermocouple temperature
// 1 thermocouple max6675 for ambient temperature
// SD Card (see if you could interrupt every minute to write to the SD) 
// User input for changing the temperature of different plates
// humidity sensor input and logic
// another PID for the cooled target

//this is the main source file 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <math.h>
#include "max6675.h"
#include "PID_v1.h"
#include "PID_AutoTune_v0.h"

// structure defining the data for a target
struct target
{
	int currentTemp;    // current temperature of the blackbody
	int prevTemp;       // previous temperature of the blackbody
	int setTemp;        // temperature set by the user in absolute temperature
	int absoluteTemp;   // initialized in setup 
	int relativeTemp;   // initialize relative temp to zero
	int targetTempMode; // the temperature mode of the system is initiliazed to absolute temperature
	double error;       // the difference between setpoint and current temperature
};

// Values to increase readability
// These values help indicate whether or not a button is pressed
const int PRESSED = LOW;
const int NOT_PRESSED = HIGH;
// These values indicate which mode the controller is currently in
const int ABS_TEMP = 1;
const int REL_TEMP = 2;
const int ERROR_MD = 3;
// This value is the predefined setpoint temperature
const int PREDEFINED_TEMP_SETPOINT_HEATED = 23;
const int PREDEFINED_TEMP_SETPOINT_COOLED = 19;
// tuning variables for the PID loop
const int TUNING_P_HEATED = 8;
const int TUNING_I_HEATED = 15;
const int TUNING_D_HEATED = 3;
const int TUNING_P_COOLED = 8;
const int TUNING_I_COOLED = 15;
const int TUNING_D_COOLED = 3;

// target variables
const int HEATED_TARGET = 0;
const int COOLED_TARGET = 1;

// PIN Allocation
// Create Fritzing Pinout Diagram
// output pins
const int HEAT_PIN = 9;                // output to heater
const int COOL_PIN = 10;               // output to cooling element

// input temperature pins
const int BB_TEMP_PIN = 2;             // input blackbody temp
const int AMBIENT_TEMP_PIN = 12;       // input ambient temp

// user input pins
const int INCREASE_TEMP_PIN = 7;       // input to increase temperature
const int DECREASE_TEMP_PIN = 8;       // input to decrease temperature
const int CHANGE_MODE_PIN = 11;        // input to change temperature mode eg from absolute to relative or vice versa
const int CHANGE_TARGET_PIN = 22;      // input to change which target we adjust the temperature of

//setup for thermoCouple Pins Amps 
// thermocouple pins for blackbody temperature
const int THERMO_DO_BB_PIN_HEATED = 4;
const int THERMO_CS_BB_PIN_HEATED = 5;
const int THERMO_CLK_BB_PIN_HEATED = 6;

const int THERMO_DO_BB_PIN_COOLED = 30;
const int THERMO_CS_BB_PIN_COOLED = 31;
const int THERMO_CLK_BB_PIN_COOLED = 32;

// thermocouple pins for ambient temperature
const int THERMO_DO_AMB_PIN = 13;
const int THERMO_CS_AMB_PIN = 14;
const int THERMO_CLK_AMB_PIN = 15;

// Global Variables
//Define PID Variables
double setpoint_cool, setpoint_heat, input_cool, input_heat, output_heat, output_cool;

// current target whom data is being manipulated
target blackbodies[2];
int currentTarget;

// Global objects
// LCD Display Object
LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x3F for a 20 chars and 4 line display
// Thermocouple Objects
MAX6675 thermocouple_blackbody_heated(THERMO_CLK_BB_PIN_HEATED, THERMO_CS_BB_PIN_HEATED, THERMO_DO_BB_PIN_HEATED);
MAX6675 thermocouple_blackbody_cooled(THERMO_CLK_BB_PIN_COOLED, THERMO_CS_BB_PIN_COOLED, THERMO_DO_BB_PIN_COOLED);
MAX6675 thermocouple_ambient(THERMO_CLK_AMB_PIN, THERMO_CS_AMB_PIN, THERMO_DO_AMB_PIN);
// Specify the links and initial tuning parameters
// PID Object
PID PIDCooledTarget(&input_cool, &output_cool, &setpoint_cool,TUNING_P_COOLED, TUNING_I_COOLED, TUNING_D_COOLED, DIRECT);
PID PIDHeatedTarget(&input_heat, &output_heat, &setpoint_heat,TUNING_P_HEATED, TUNING_I_HEATED, TUNING_D_HEATED, DIRECT);

void setup() 
{

	// setup user input
	userInputSetup();
	//setup LCD display
	LCDSetup();
	//set TE-COOLER output pin 
	pinMode(HEAT_PIN, OUTPUT);
	pinMode(COOL_PIN, OUTPUT);
	// initialize the thermocouples
	thermoInit();  // what does this do? Do we only need it once no matter how many thermocouples there are?
	// set the input and setpoint of the PID loop
	pidSetup();

	// set up global variables
	// temperatures set to predefined setpoint
	//
	currentTarget = HEATED_TARGET; // by default we modify the heated target
	// 
	blackbodies[HEATED_TARGET].setTemp = PREDEFINED_TEMP_SETPOINT_HEATED;
	blackbodies[HEATED_TARGET].absoluteTemp = PREDEFINED_TEMP_SETPOINT_HEATED;
	blackbodies[HEATED_TARGET].currentTemp = thermocouple_blackbody_heated.readCelsius();
	blackbodies[HEATED_TARGET].prevTemp = thermocouple_blackbody_heated.readCelsius();
	blackbodies[HEATED_TARGET].relativeTemp = 0;
	blackbodies[HEATED_TARGET].error = blackbodies[HEATED_TARGET].currentTemp - blackbodies[HEATED_TARGET].setTemp;
	// system mode initialized to absolute temperature
	blackbodies[HEATED_TARGET].targetTempMode = ABS_TEMP;

	blackbodies[COOLED_TARGET].setTemp = PREDEFINED_TEMP_SETPOINT_COOLED;
	blackbodies[COOLED_TARGET].absoluteTemp = PREDEFINED_TEMP_SETPOINT_COOLED;
	blackbodies[COOLED_TARGET].currentTemp = thermocouple_blackbody_cooled.readCelsius();
	blackbodies[COOLED_TARGET].prevTemp = thermocouple_blackbody_cooled.readCelsius();
	blackbodies[COOLED_TARGET].relativeTemp = 0;
	blackbodies[COOLED_TARGET].error = blackbodies[COOLED_TARGET].currentTemp - blackbodies[COOLED_TARGET].setTemp;
	// system mode initialized to absolute temperature
	blackbodies[COOLED_TARGET].targetTempMode = ABS_TEMP;

	// start serial
	Serial.begin(9600);
}

void loop() {
	
    // the current temperature of the blackbody
	blackbodies[COOLED_TARGET].currentTemp = thermocouple_blackbody_cooled.readCelsius();
	blackbodies[HEATED_TARGET].currentTemp = thermocouple_blackbody_heated.readCelsius();

	// current ambient temperature
	int currentAmbientTemp = int(thermocouple_ambient.readCelsius());
    currentAmbientTemp = 25;  // $$ for testing purposes
    
	// get the user input for the temperature
	userInputLoopTargetChange(currentTarget);
	userInputLoop(blackbodies[currentTarget].targetTempMode, blackbodies[currentTarget].relativeTemp, blackbodies[currentTarget].absoluteTemp); // values are passed by reference and modified
	
	// depending on the user input set the setpoint
	setSetpoint(blackbodies[currentTarget].setTemp, blackbodies[currentTarget].targetTempMode, blackbodies[currentTarget].relativeTemp, currentAmbientTemp, blackbodies[currentTarget].absoluteTemp);
	
	// display the temperature setpoint
	dispSetTemp(blackbodies[currentTarget].setTemp);

	//Serial.println(thermocouple.readCelsius());
	  
	delay(500); 

	// display the current blackbody temp
	dispCurrentTemp(blackbodies[currentTarget].currentTemp);	

	// use the PID to calculate an output value to the heating/cooling sources
	pidCalc();


	// display the error to the LCD
	blackbodies[COOLED_TARGET].error = blackbodies[COOLED_TARGET].currentTemp - blackbodies[COOLED_TARGET].setTemp;
	blackbodies[HEATED_TARGET].error = blackbodies[HEATED_TARGET].currentTemp - blackbodies[HEATED_TARGET].setTemp;
	dispError(blackbodies[currentTarget].error); 
  
	// for averaging from last loop to reduce spikes, could be elimiated 
	blackbodies[COOLED_TARGET].prevTemp = blackbodies[COOLED_TARGET].currentTemp;
	blackbodies[HEATED_TARGET].prevTemp = blackbodies[HEATED_TARGET].currentTemp;

	// output the heating and cooling elements
	tempCon(); 
	
	// display the PWM value
	if (currentTarget == HEATED_TARGET)
	{
		disPWM(output_heat);
	}
	else if (currentTarget == COOLED_TARGET)
	{
		disPWM(output_cool);
	}
	else
	{
		Serial.println("Error: Cannot display output for PWM value!");
	}
}
