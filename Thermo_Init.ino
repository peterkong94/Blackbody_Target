/*    thermoInit
Description:	This is the thermocouple initialization routine.
				It should only be once in setup. 
Input:			none
Output:			none
*/
void thermoInit() {
  Serial.println("MAX6675 test");
  // wait for MAX chip to stabilize
  delay(500);
}

