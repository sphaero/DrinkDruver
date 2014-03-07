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
FSM stateMachine = FSM(Sober); //initialize state machine, start in state: Sober

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

#define BTN_TEST 11

// Controller pins
#define GO_RIGHT 3
#define GO_FWD 5
#define GO_BWD 7
#define GO_LEFT 9

void setup()
{
  Serial.begin(9600);
  //random entropy
  randomSeed(analogRead(0));
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
  pinMode(BTN_TEST, INPUT_PULLUP);
}

int fwd, bwd, left, right, test;

int readMq3()
{
  int mq3_value = analogRead(mq3_analogPin);
  //Serial.print("mq3_value: ");
  //Serial.print(mq3_value);
  //Serial.print("\r\n");
  if (mq3_value < 90 )
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
  test  = digitalRead(BTN_TEST);
}


void loop()
{
  readButtons();
  if (!test)
  {
    Serial.println("testing sensor");
    readMq3();
  }
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

float delta = 1.0;
void drunkUpdate()
{
  // extreme unstable control
  float slowsin = sin(millis()/270);
  float fastsin = sin(millis()/13);
  if (!fwd) {
    //sometimes we forget to let go of the pedal :)
    int rnd = random(0,100)/90.0;
    Serial.println(rnd);
    if (rnd)
    {
      delta = 0.0;
    }
    digitalWrite(GO_FWD, LOW);
    // some times go left or right
    if (slowsin > 0.5 && left) left = LOW;
    else if (slowsin < -0.5 && right) right = LOW;
  }
  else if (!bwd)
  {
    digitalWrite(GO_BWD, LOW);
    if (fastsin > 0.5 && left) left = LOW;
    else if (fastsin < -0.5 && right) right = LOW;    
  }
  else
  {
    delta+=0.02;
    Serial.println(delta);
    digitalWrite(GO_FWD, (int)delta);
    digitalWrite(GO_BWD, (int)delta);
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


int *controls[] = { &fwd, &bwd, &left, &right};

void hammeredUpdate()
{  
  // complete mixup of controls
  if (millis() % 5000 < 1) 
  {
    Serial.println("shuffled");
    shuffle(controls); 
  }
 
  if (!*controls[0]) {

    digitalWrite(GO_FWD, LOW);
  }
  else if (!*controls[1]) 
  {
    digitalWrite(GO_BWD, LOW);
  }
  else
  {
    digitalWrite(GO_FWD, HIGH);
    digitalWrite(GO_BWD, HIGH);
  }

  if (!*controls[2]) {
    digitalWrite(GO_LEFT, LOW);
  }
  else if (!*controls[3]) {
    digitalWrite(GO_RIGHT, LOW);
  }
  else
  {
    digitalWrite(GO_LEFT, HIGH);
    digitalWrite(GO_RIGHT, HIGH);
  }
}

void shuffle(int* array[])
{
  int n = 4;
  
  for (int i = 0; i< n - 1; i++)
  {
    int j = random(0,39)/10;//i + rand() / (RAND_MAX / (n - i) + 1);
    int* t = array[j];
    array[j] = array[i];
    array[i] = t;
  }
}
