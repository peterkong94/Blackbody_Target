

// 8 thermocouples max31856 .h and .cpp (Google MAX31856) for thermocouple temperature
// 1 thermocouple max6675 for ambient temperature
// SD Card (see if you could interrupt every minute to write to the SD) 
// User input for changing the temperature of different plates
// humidity sensor input and logic
// another PID for the cooled target

//this is the main source file 
#include <SPI.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <math.h>
#include "Adafruit_HTU21DF.h"
#include "Adafruit_MAX31856.h"
#include "max6675.h"
#include "PID_v1.h"
#include "PID_AutoTune_v0.h"

// structure defining the data for a target
struct target
{
	float currentTemp;    // current temperature of the blackbody
	//float prevTemp;       // previous temperature of the blackbody
	int setTemp;          // temperature set by the user in absolute temperature
	int absoluteTemp;     // initialized in setup 
	int relativeTemp;     // initialize relative temp to zero
	int targetTempMode;   // the temperature mode of the system is initiliazed to absolute temperature
	double error;         // the difference between setpoint and current temperature
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
const int TUNING_P_COOLED = 8;
const int TUNING_I_COOLED = 15;
const int TUNING_D_COOLED = 3;

// target variables
const int HEATED_TARGET = 0;
const int COOLED_TARGET = 1;

// PIN Allocation
// output pins

const int T9_PWM = 5;
const int T8_PWM = 6;
const int T7_PWM = 7;
const int T6_PWM = 8;
const int T5_PWM = 9;
const int T4_PWM = 10;
const int T3_PWM = 11;
const int T2_PWM = 12;
const int T1_PWM = 13;

// user input pins
const int INCREASE_TEMP_PIN = 40;       // input to increase temperature
const int DECREASE_TEMP_PIN = 41;       // input to decrease temperature
const int CHANGE_MODE_PIN = 42;        // input to change temperature mode eg from absolute to relative or vice versa
const int CHANGE_TARGET_PIN = 43;      // input to change which target we adjust the temperature of

//setup for thermoCouple Pins Amps 
// thermocouple pins for blackbody temperature

//const int THERMO_DO_BB_PIN = 30;
const int THERMO_GROUP1_SCK = 22;
const int THERMO_GROUP1_SDO = 23;
const int THERMO_GROUP1_SDI = 24;
//const int THERMO_CLK_BB_PIN = 32;

//const int THERMO_CS_PIN_1 = 31;
const int THERMO1_ICS = 25;
const int THERMO2_ICS = 26;
const int THERMO3_ICS = 27;
const int THERMO4_ICS = 28;

// Global Variables
//Define PID Variables
double setpoint_cool, input_cool, output_cool;
double relative_humidity;
double currentAmbientTemp;

// current target whom data is being manipulated
target blackbodies[2];
int currentTarget;

// Global objects
// LCD Display Object
LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x3F for a 20 chars and 4 line display

								   
// Thermocouple Objects
//MAX6675 thermocouple_blackbody_cooled(THERMO_CLK_BB_PIN_COOLED, THERMO_CS_BB_PIN_COOLED, THERMO_DO_BB_PIN_COOLED);
Adafruit_MAX31856 Thermo_1(THERMO1_ICS, THERMO_GROUP1_SDI, THERMO_GROUP1_SDO, THERMO_GROUP1_SCK);
Adafruit_MAX31856 Thermo_2(THERMO2_ICS, THERMO_GROUP1_SDI, THERMO_GROUP1_SDO, THERMO_GROUP1_SCK);
Adafruit_MAX31856 Thermo_3(THERMO3_ICS, THERMO_GROUP1_SDI, THERMO_GROUP1_SDO, THERMO_GROUP1_SCK);
Adafruit_MAX31856 Thermo_4(THERMO4_ICS, THERMO_GROUP1_SDI, THERMO_GROUP1_SDO, THERMO_GROUP1_SCK);

// Humidity Sensor Object
Adafruit_HTU21DF Humidity_Sensor = Adafruit_HTU21DF();

// Specify the links and initial tuning parameters
// PID Object
PID PIDCooledTarget(&input_cool, &output_cool, &setpoint_cool,TUNING_P_COOLED, TUNING_I_COOLED, TUNING_D_COOLED, DIRECT);


void setup() 
{

	// setup user input
	userInputSetup();
	//setup LCD display
	LCDSetup();
	//set TE-COOLER output pin 
	te_cooler_pin_setup();
	// initialize the thermocouples
	Thermo_1.setThermocoupleType(MAX31856_TCTYPE_T);
	Thermo_2.setThermocoupleType(MAX31856_TCTYPE_T);
	Thermo_3.setThermocoupleType(MAX31856_TCTYPE_T);
	Thermo_4.setThermocoupleType(MAX31856_TCTYPE_T);
	thermoInit();  // what does this do? Do we only need it once no matter how many thermocouples there are?

	// set up humidity sensor
	Humidity_Sensor.begin();

	// set the input and setpoint of the PID loop
	pidSetup();

	// set up global variables
	// temperatures set to predefined setpoint
	//
	currentTarget = HEATED_TARGET; // by default we modify the heated target
	// 
	blackbodies[HEATED_TARGET].setTemp = PREDEFINED_TEMP_SETPOINT_HEATED;
	blackbodies[HEATED_TARGET].absoluteTemp = PREDEFINED_TEMP_SETPOINT_HEATED;
	//blackbodies[HEATED_TARGET].prevTemp = 0;
	blackbodies[HEATED_TARGET].relativeTemp = 0;
	blackbodies[HEATED_TARGET].error = blackbodies[HEATED_TARGET].currentTemp - blackbodies[HEATED_TARGET].setTemp;
	// system mode initialized to absolute temperature
	blackbodies[HEATED_TARGET].targetTempMode = ABS_TEMP;

	blackbodies[COOLED_TARGET].setTemp = PREDEFINED_TEMP_SETPOINT_COOLED;
	blackbodies[COOLED_TARGET].absoluteTemp = PREDEFINED_TEMP_SETPOINT_COOLED;
	blackbodies[COOLED_TARGET].currentTemp = Thermo_1.readThermocoupleTemperature();
	//blackbodies[COOLED_TARGET].prevTemp = thermocouple_blackbody_cooled.readCelsius();
	blackbodies[COOLED_TARGET].relativeTemp = 0;
	blackbodies[COOLED_TARGET].error = blackbodies[COOLED_TARGET].currentTemp - blackbodies[COOLED_TARGET].setTemp;
	// system mode initialized to absolute temperature
	blackbodies[COOLED_TARGET].targetTempMode = ABS_TEMP;

	// start serial
	Serial.begin(9600);
	Serial1.begin(9600);
	Serial1.setTimeout(100);
}

void loop() {
	
	float temp;

    // the current temperature of the blackbody
	blackbodies[COOLED_TARGET].currentTemp = Thermo_1.readThermocoupleTemperature();
	currentAmbientTemp = Humidity_Sensor.readTemperature();
	relative_humidity = Humidity_Sensor.readHumidity();

	/*
	Serial.println("The current temperature and relative humidity is:");
	Serial.println(currentAmbientTemp);
	Serial.println(relative_humidity);
	Serial.println();
	*/

	// get ambient temperature and humidity
	

	int iterations = 0;
	
	while (Serial1.available() && iterations <= 100)
	{
		//Serial.println("Read a value from UNO.");
		// read string then convert it to a float
		temp = Serial1.parseFloat();
		if (int(temp) != 0) {
			blackbodies[HEATED_TARGET].currentTemp = temp;
			iterations++;
			//Serial.println(blackbodies[HEATED_TARGET].currentTemp);
		}
	}

	// current ambient temperature
	
    
	// get the user input for the temperature
	userInputLoopTargetChange(currentTarget);
	userInputLoop(blackbodies[currentTarget].targetTempMode, blackbodies[currentTarget].relativeTemp, blackbodies[currentTarget].absoluteTemp); // values are passed by reference and modified
	
	/*
	Serial.println();
	Serial.println(blackbodies[HEATED_TARGET].setTemp);
	Serial.println(blackbodies[HEATED_TARGET].absoluteTemp);
	Serial.println(blackbodies[HEATED_TARGET].relativeTemp);
	Serial.println(currentAmbientTemp);	
	Serial.println();
	*/

	// depending on the user input set the setpoint
	setSetpoint(blackbodies[COOLED_TARGET].setTemp, blackbodies[COOLED_TARGET].targetTempMode, blackbodies[COOLED_TARGET].relativeTemp, currentAmbientTemp, blackbodies[COOLED_TARGET].absoluteTemp);
	setSetpoint(blackbodies[HEATED_TARGET].setTemp, blackbodies[HEATED_TARGET].targetTempMode, blackbodies[HEATED_TARGET].relativeTemp, currentAmbientTemp, blackbodies[HEATED_TARGET].absoluteTemp);

	// check dew point and make sure the setpoints don't go below the dew point. 

	/*
	Serial.println();
	Serial.println(currentAmbientTemp);
	Serial.println(relative_humidity);
	Serial.println(calculateDewPoint());
	Serial.println();
	*/

	setpointDewPoint();

	// send the setpoint to the other Arduino
	Serial1.println(blackbodies[HEATED_TARGET].setTemp);
	Serial.println(blackbodies[HEATED_TARGET].setTemp);

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
	//blackbodies[COOLED_TARGET].prevTemp = blackbodies[COOLED_TARGET].currentTemp;
	//blackbodies[HEATED_TARGET].prevTemp = blackbodies[HEATED_TARGET].currentTemp;

	// output the heating and cooling elements
	tempCon(); 
	
	// display the PWM value
	if (currentTarget == HEATED_TARGET)
	{
		//disPWM(output_heat);
		disPWM(-10); // the output of the PID is at the UNO controller
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
