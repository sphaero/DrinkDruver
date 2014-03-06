/******
*
* Simple car controller just mimicking the native controller
*
*
*
******/

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
  digitalWrite(GO_FWD, LOW);
  digitalWrite(GO_BWD, LOW);
  digitalWrite(GO_LEFT, LOW);
  digitalWrite(GO_RIGHT, LOW);

  pinMode(BTN_FWD, INPUT_PULLUP);
  pinMode(BTN_BWD, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
}

int fwd, bwd, left, right;

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
  if (!fwd) {
    Serial.println("FWD");
    digitalWrite(GO_BWD, HIGH);
  }
  else
  {
    digitalWrite(GO_FWD, LOW);
  }

  if (!bwd) {
    Serial.println("BWD");
    digitalWrite(GO_BWD, HIGH);
  }
  else
  {
    digitalWrite(GO_BWD, LOW);
  }

  if (!left) {
    Serial.println("LEFT");
    digitalWrite(GO_LEFT, HIGH);
  }
  else
  {
    digitalWrite(GO_LEFT, LOW);
  }

  if (!right) {
    Serial.println("RIGHT");
    digitalWrite(GO_RIGHT, HIGH);
  }
  else
  {
    digitalWrite(GO_RIGHT, LOW);
  }
}

