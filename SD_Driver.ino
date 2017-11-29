/* nothing in here yet, 
we could skip the SD card write function for the arduino protoltype, 
what do you guys think?
*/

void log_data()
{
	sdFile = SD.open("log.csv", FILE_WRITE);

	// need more temperatures on the target
	// possible call thermocouples asking for temperature
	// problem will be with heated target since the thermocouple data will be on other microcontroller

	double sum = 0;

	// print the header that tells the the title of each data point

	sum += sdFile.print(currentAmbientTemp);
	sum += sdFile.print(", ");
	sum += sdFile.print(relative_humidity);
	sum += sdFile.print(", ");
	sum += sdFile.print(calculateDewPoint());
	sum += sdFile.print(", ");
	sum += sdFile.print(Thermo_1.readThermocoupleTemperature());
	sum += sdFile.print(", ");
	sum += sdFile.print(currentAmbientTemp);
	sum += sdFile.print(", ");
	sum += sdFile.print(currentAmbientTemp);
	sum += sdFile.print(", ");
	sum += sdFile.print(currentAmbientTemp);
	sum += sdFile.print(", ");
	sum += sdFile.print(currentAmbientTemp);
	sum += sdFile.print(", ");
	sum += sdFile.print(currentAmbientTemp);
	sum += sdFile.print(", ");
	sum += sdFile.print(currentAmbientTemp);
	sum += sdFile.print(", ");
	sum += sdFile.println(currentAmbientTemp);
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
}
