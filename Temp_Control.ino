/********************************************************
 * PID Basic Example
 * Reading analog input 0 to control analog PWM output 3
 * This is only for reference and not called right now 
 ********************************************************/
void PIDSetup()
{
  Input = thermocouple.readCelsius();
  Setpoint = 0;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
}

void PIDCalc(double currentTemp, int setTemp)
{
  Setpoint = setTemp;
  Input = currentTemp;
  myPID.Compute();
  //analogWrite(3,Output);
}

void TempCon()
{
  int OnTime = 20*Output; 
  digitalWrite(heatPin, HIGH); 
  delay(OnTime); 
  digitalWrite(heatPin, LOW); 
  delay(5000 - OnTime); 
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

