//Pin 2,3, 18, 19, 20, 21 are the interrupts pin on Mega 
const int change_temp_pin = 2;
const int change_mode_pin = 3;
int tempButtonState, modeButtonState;
const int PRESSED = LOW;

enum temperature_modes {
  ABS_TEMP,
  REL_TEMP, 
  ERROR_MD  
};



temperature_modes change_modes(temperature_modes original_mode)
{
  if( original_mode == ABS_TEMP)
  {
    return REL_TEMP;
  }
  else if( original_mode == REL_TEMP)
  {
    return ABS_TEMP;
  }
  else
  {
    return ERROR_MD;
  }
}

temperature_modes user_input_loop(temperature_modes temp_mode, int& rel_temp, int& abs_temp, int current_temp)
{
  
  tempButtonState = digitalRead(change_temp_pin);
  modeButtonState = digitalRead(change_mode_pin);

  if(modeButtonState == PRESSED)
  {
    temp_mode = change_modes(temp_mode);
    
  }
  if(temp_mode == REL_TEMP)
  {
    if(tempButtonState == PRESSED)
    {
      if(rel_temp == 15)
      {
        rel_temp = 0;
      }
      else
      {
        rel_temp = rel_temp + 1;
      }
    }
  }
  else if(temp_mode == ABS_TEMP)
  {
    if(tempButtonState == PRESSED)
    {
      if(abs_temp == current_temp+15)
      {
        abs_temp = current_temp;
      }
      else
      {
        abs_temp = abs_temp + 1;
      }
    }
  }
  else
  {
    Serial.println("Error! Incorrect Temperature Mode");  
  }
 
  return temp_mode; 
}

void user_input_setup()
{
  //this is the interrupt setup routine 
  pinMode(change_temp_pin, INPUT_PULLUP);
  pinMode(change_mode_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(change_temp_pin), user_input_loop, CHANGE);
  attachInterrupt(digitalPinToInterrupt(change_mode_pin), user_input_loop, CHANGE);
}
