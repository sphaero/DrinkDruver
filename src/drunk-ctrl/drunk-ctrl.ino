/******
*
* Simple car controller just mimicking the native controller
*
*
*
******/
#include "FiniteStateMachine.h"
// how many states are we cycling through?
const byte NUMBER_OF_DRUNK_STATES = 4;
// define states
State Sober = State(soberUpdate);
State Tipsy = State(tipsyUpdate);
State Drunk = State(drunkUpdate);
State Hammered = State(hammeredUpdate);

// the state machine controls which of the states get attention and execution time
FSM stateMachine = FSM(Hammered); //initialize state machine, start in state: Sober

/* 
  It takes about 20-30 minutes before the values become more
  stable. Once the value is a bit more stable (for example around
  100), the value for the "sober state" should be around 120.
  Most of the time whatever the value is, breathing onto the
  sensor increases it by 20.
  
  Around 50-60, it becomes very stable.
  
  The value increase by 100-200 with a bit of alcohol if you
  breathe on the sensor right after drinking. If you do it again
  after 5 minutes or so, the value starts increasing much less.
*/
int mq3_analogPin = A5; // connected to the output pin of MQ3

// Controller buttons
#define BTN_RIGHT 2
#define BTN_FWD 4
#define BTN_BWD 6
#define BTN_LEFT 8

// Controller pins
#define GO_RIGHT 3
#define GO_FWD 5
#define GO_BWD 7
#define GO_LEFT 9

void setup()
{
  Serial.begin(9600);
  pinMode(GO_FWD, OUTPUT);
  pinMode(GO_BWD, OUTPUT);
  pinMode(GO_LEFT, OUTPUT);
  pinMode(GO_RIGHT, OUTPUT);
  digitalWrite(GO_FWD, HIGH);
  digitalWrite(GO_BWD, HIGH);
  digitalWrite(GO_LEFT, HIGH);
  digitalWrite(GO_RIGHT, HIGH);

  pinMode(BTN_FWD, INPUT_PULLUP);
  pinMode(BTN_BWD, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
}

int fwd, bwd, left, right;

int readMq3()
{
  int mq3_value = analogRead(mq3_analogPin);
  //Serial.print("mq3_value: ");
  //Serial.print(mq3_value);
  //Serial.print("\r\n");
  if (mq3_value < 60 )
  {
    //sober
    stateMachine.transitionTo(Sober);
    return 0;
  }
  if (mq3_value < 120)
  {
    //tipsy
    stateMachine.transitionTo(Tipsy);
    return 1;
  }
  if (mq3_value < 150)
  {
    //drunk
    stateMachine.transitionTo(Drunk);
    return 2;
  }
  else 
  {
    //hammered
    stateMachine.transitionTo(Hammered);
    return 3;
  }  
}

void readButtons()
{
  fwd   = digitalRead(BTN_FWD); 
  bwd   = digitalRead(BTN_BWD); 
  left  = digitalRead(BTN_LEFT); 
  right = digitalRead(BTN_RIGHT); 
}


void loop()
{
  readButtons();
  readMq3();
  stateMachine.update();
}

void soberUpdate() 
{
  if (!fwd) {
    digitalWrite(GO_FWD, LOW);
  }
  else if (!bwd) 
  {
    digitalWrite(GO_BWD, LOW);
  }
  else
  {
    digitalWrite(GO_FWD, HIGH);
    digitalWrite(GO_BWD, HIGH);
  }

  if (!left) {
    digitalWrite(GO_LEFT, LOW);
  }
  else if (!right) {
    digitalWrite(GO_RIGHT, LOW);
  }
  else
  {
    digitalWrite(GO_LEFT, HIGH);
    digitalWrite(GO_RIGHT, HIGH);
  }
}

void tipsyUpdate()
{
  float lr = sin(millis()/100);
  if (!fwd) {
    digitalWrite(GO_FWD, LOW);
    // some times go left or right
    if (lr > 0.7 && left) left = LOW;
    else if (lr < -0.7 && right) right = LOW;
  }
  else if (!bwd)
  {
    digitalWrite(GO_BWD, LOW);
  }
  else
  {
    digitalWrite(GO_FWD, HIGH);
    digitalWrite(GO_BWD, HIGH);
  }


  if (!left) 
  {
    digitalWrite(GO_LEFT, LOW);
  }
  else if (!right) 
  {
    digitalWrite(GO_RIGHT, LOW);
  }
  else
  {
    digitalWrite(GO_LEFT, HIGH);
    digitalWrite(GO_RIGHT, HIGH);
  }
}

void drunkUpdate()
{}

void hammeredUpdate()
{
  int reverse = round(sin(millis()));
  if (!fwd) // || (!bwd && reverse)) 
  {
    Serial.println("FWD");
    digitalWrite(GO_BWD, LOW);
  }
  else
  {
    digitalWrite(GO_BWD, HIGH);
  }

  if (!bwd)// || (!fwd && reverse)) 
  {
    Serial.println("BWD");
    digitalWrite(GO_FWD, LOW);
  }
  else
  {
    digitalWrite(GO_FWD, HIGH);
  }

  if (!left) {
    digitalWrite(GO_RIGHT, LOW);
  }
  else
  {
    digitalWrite(GO_RIGHT, HIGH);
  }

  if (!right) {
    digitalWrite(GO_LEFT, LOW);
  }
  else
  {
    digitalWrite(GO_LEFT, HIGH);
  }
}
