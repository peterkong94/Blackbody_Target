/*

#define TEMPUP_PIN               6  // temp up
#define TEMPDOWN_PIN             7  // temp down 

//this event handler needs to detect button press and button relase 
//when button pressed do setTemp++/-- 
//when button relased resume code 

//////////////////////////////////////////////////////////////////////////////

enum { EV_NONE=0, EV_PRESSED, EV_RELEASED};

//////////////////////////////////////////////////////////////////////////////
// Class definition

class ButtonHandler {
  public:
    // Constructor
    ButtonHandler(int pin);

    // Initialization done after construction, to permit static instances
    void init();

    // Handler, to be called in the loop()
    int handle();

  protected:
    boolean was_pressed;     // previous state
    int pressed_counter;     // press running duration
    const int pin;           // pin to which button is connected
};

ButtonHandler::ButtonHandler(int p)
: pin(p)
{
  setTemp--;
}

void ButtonHandler::init()
{
  pinMode(pin, INPUT);
  //digitalWrite(pin, LOW); // pull-up
  was_pressed = false;
  pressed_counter = 0;
}

int ButtonHandler::handle()
{
  int event;
  int now_pressed = digitalRead(pin);

  if (!now_pressed && was_pressed){
    // handle release event
    event = EV_RELEASED;
    Serial.print("released");
  } 
  else
    event = EV_NONE;

  // update press running duration
  if (now_pressed){
    ++pressed_counter;
  }
  else
    pressed_counter = 0;
    setTemp += pressed_counter;
  // remember state, and we're done
  was_pressed = now_pressed;
  return event;
}

//////////////////////////////////////////////////////////////////////////////

// Instantiate button objects
ButtonHandler button1(TEMPUP_PIN);
ButtonHandler button2(TEMPDOWN_PIN);

void print_event(const char* button_name, int event)
{
  if (event)
    Serial.print(button_name);
    Serial.print(".SL"[event]);
}

void eventInit()
{
  // init buttons pins; I suppose it's best to do here
  button1.init();
  button2.init();
}

void eventHandle()
{
  // handle button
  int event1 = button1.handle();
  int event2 = button2.handle();

  // do other things
  print_event("1", event1);
  print_event("2", event2);

}

*/