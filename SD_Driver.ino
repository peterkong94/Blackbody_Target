/* nothing in here yet, 
we could skip the SD card write function for the arduino protoltype, 
what do you guys think?
*/

void log_data()
{
	static int writes = 0;

	sdFile = SD.open("log.csv", FILE_WRITE);

	double sum = 0;

	if (writes == 0)
	{
		sum += sdFile.print("Ambient Temperature");
		sum += sdFile.print(", ");
		sum += sdFile.print("Relative Humidity");
		sum += sdFile.print(", ");
		sum += sdFile.print("Dew Point");
		sum += sdFile.print(", ");
		sum += sdFile.print("Cooled Target: Temp1");
		sum += sdFile.print(", ");
		sum += sdFile.print("Cooled Target: Temp2");
		sum += sdFile.print(", ");
		sum += sdFile.print("Cooled Target: Temp3");
		sum += sdFile.print(", ");
		sum += sdFile.print("Cooled Target: Temp4");
		sum += sdFile.print(", ");
		sum += sdFile.print("Heated Target: Temp1");
		sum += sdFile.print(", ");
		sum += sdFile.print("Heated Target: Temp2");
		sum += sdFile.print(", ");
		sum += sdFile.print("Heated Target: Temp3");
		sum += sdFile.print(", ");
		sum += sdFile.println("Heated Target: Temp4");
	}

	if (!sdFile)
	{
		Serial.println("File would not open!");
	}
	// need more temperatures on the target
	// possible call thermocouples asking for temperature
	// problem will be with heated target since the thermocouple data will be on other microcontroller	

	// print the header that tells the the title of each data point

	sum += sdFile.print(currentAmbientTemp);
	sum += sdFile.print(", ");
	sum += sdFile.print(relative_humidity);
	sum += sdFile.print(", ");
	sum += sdFile.print(calculateDewPoint());
	sum += sdFile.print(", ");
	//sum += sdFile.print(Thermo_1.readThermocoupleTemperature());
	//sum += sdFile.print(", ");
	sum += sdFile.print(currentAmbientTemp);
	sum += sdFile.print(", ");
	sum += sdFile.print(currentAmbientTemp);
	sum += sdFile.print(", ");
	sum += sdFile.print(currentAmbientTemp);
	sum += sdFile.print(", ");
	sum += sdFile.print(temp1);
	sum += sdFile.print(", ");
	sum += sdFile.print(temp2);
	sum += sdFile.print(", ");
	sum += sdFile.print(temp3);
	sum += sdFile.print(", ");
	sum += sdFile.println(temp4);
	/*
	sdFile.print(blackbodies[HEATED_TARGET].currentTemp);
	sdFile.print(", ");
	sdFile.println(blackbodies[COOLED_TARGET].currentTemp);
	*/

	sdFile.close();

	Serial.println();
	Serial.print("The sum of bytes printed in one log is:");
	Serial.print(sum);
	Serial.println();

	writes = writes + 1;
}
