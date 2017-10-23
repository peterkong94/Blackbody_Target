//this is the main source file 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
<<<<<<< HEAD
LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x3F for a 20 chars and 4 line display
int heatPin = 9;           // the PWM pin the LED is attached to
int coolPin = 10;    // how bright the LED is

#include "user_input.h"

=======
>>>>>>> 8dd340bc486ab0b566518ef705867e67830c2770
#include <math.h>
#include "max6675.h"
#include "PID_v1.h"
#include "PID_AutoTune_v0.h"
<<<<<<< HEAD
//Define Variables we'll be connecting to
double Setpoint, Input, Output;
//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint,8,15,3, DIRECT);

void setup() {
  // put your setup code here, to run once
  //setup LCD display
  LCDSetup();
  pinMode(heatPin, OUTPUT);
  pinMode(coolPin, OUTPUT);
  prevTemp = getCurrentTemp();
  thermoInit();
  PIDSetup();

  user_input_setup();
  
  Serial.begin(9600);
}

void loop() {

  //Serial.print(digitalRead(TEMPUP_PIN));
  //Serial.print("/");
  dispSetTemp(setTemp);
  // put your main code here, to run repeatedly:
  //Serial.println(thermocouple.readCelsius());
  currentTemp = thermocouple.readCelsius();  
  delay(500); 
  dispCurrentTemp(currentTemp);
  PIDCalc(currentTemp, setTemp);
=======

// Values to increase readability
// These values help indicate whether or not a button is pressed
const int PRESSED = LOW;
const int NOT_PRESSED = HIGH;
// These values indicate which mode the controller is currently in
const int ABS_TEMP = 1;
const int REL_TEMP = 2;
const int ERROR_MD = 3;
// This value is the predefined setpoint temperature
const int PREDEFINED_TEMP_SETPOINT = 23;
// tuning variables for the PID loop
const int TUNING_P = 8;
const int TUNING_I = 15;
const int TUNING_D = 3;

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
const int CHANGE_MODE_PIN = 11;        // input to change temperature mode
                                       // eg from absolute to relative or vice versa

//setup for thermoCouple Pins Amps 
// thermocouple pins for blackbody temperature
const int THERMO_DO_BB_PIN = 4;
const int THERMO_CS_BB_PIN = 5;
const int THERMO_CLK_BB_PIN = 6;

// thermocouple pins for ambient temperature
const int THERMO_DO_AMB_PIN = 13;
const int THERMO_CS_AMB_PIN = 14;
const int THERMO_CLK_AMB_PIN = 15;

// Global Variables
//Define PID Variables
double setpoint, input, output;
double prevTemp;         // the previous temperature of the blackbody

// Initialize temperature variables
int setTemp;             // temperature set by the user in absolute temperature
int sysTempMode;         // the temperature mode of the system is initiliazed to absolute temperature
int relativeTemp;        // initialize relative temp to zero
int absoluteTemp;        // initialized in setup

// Global objects
// LCD Display Object
LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x3F for a 20 chars and 4 line display
// Thermocouple Objects
MAX6675 thermocouple_blackbody(THERMO_CLK_BB_PIN, THERMO_CS_BB_PIN, THERMO_DO_BB_PIN);
MAX6675 thermocouple_ambient(THERMO_CLK_AMB_PIN, THERMO_CS_AMB_PIN, THERMO_DO_AMB_PIN);
// Specify the links and initial tuning parameters
// PID Object
PID myPID(&input, &output, &setpoint,TUNING_P,TUNING_I,TUNING_D, DIRECT);

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
	pidSetup(thermocouple_blackbody);

	// set up global variables
	// temperatures set to predefined setpoint
	setTemp = PREDEFINED_TEMP_SETPOINT;
	absoluteTemp = PREDEFINED_TEMP_SETPOINT;
	relativeTemp = 0;
	// system mode initialized to absolute temperature
	sysTempMode = ABS_TEMP;
	prevTemp = thermocouple_blackbody.readCelsius();

	// start serial
	Serial.begin(9600);
}

void loop() {
	
	// local variables 
	int error;       // difference between blackbody temperature and the setpoint
        // the current temperature of the blackbody
	double currentTemp = thermocouple_blackbody.readCelsius();
	// current ambient temperature
	int currentAmbientTemp = int(thermocouple_ambient.readCelsius());
        currentAmbientTemp = 25;
        
	// get the user input for the temperature
	userInputLoop(sysTempMode, relativeTemp, absoluteTemp); // values are passed by reference and modified
>>>>>>> 8dd340bc486ab0b566518ef705867e67830c2770

	// depending on the user input set the setpoint
	setSetpoint(setTemp, sysTempMode, relativeTemp, currentAmbientTemp, absoluteTemp);

	// display the temperature setpoint
	dispSetTemp(setTemp);

	//Serial.println(thermocouple.readCelsius());
	  
	delay(500); 

	// display the current blackbody temp
	dispCurrentTemp(currentTemp);

	pidCalc(currentTemp, setTemp);

	// display the error to the LCD
	error = currentTemp - setTemp;
	dispError(error); 
  
	// for averaging from last loop to reduce spikes, could be elimiated 
	prevTemp = currentTemp; 

	// display the PWM value
	disPWM(output);

	// output the heating and cooling elements
	tempCon(); 
  
}
