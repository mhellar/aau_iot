

// constants won't change. They're used here to 
// set pin numbers:
const int button1Pin = 8;   
const int button2Pin = 9;   
const int button3Pin = 10;   
const int button4Pin = 11;     


// variables will change:
int button1State = 0;         // variable for reading the pushbutton status
int button2State = 0;         // variable for reading the pushbutton status
int button3State = 0;         // variable for reading the pushbutton status
int button4State = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize the pushbutton pin as an input:
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);
  pinMode(button4Pin, INPUT_PULLUP);
  
  Serial.begin(9600);     
}

void loop(){
  // read the state of the pushbutton value:
  button1State = digitalRead(button1Pin);
  button2State = digitalRead(button2Pin);
  button3State = digitalRead(button3Pin);
  button4State = digitalRead(button4Pin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (button1State == LOW) {
    Serial.println("1");
    delay(250);     
  } 
    if (button2State == LOW) {
    Serial.println("2");
    delay(250);     
  } 
    if (button3State == LOW) {
    Serial.println("3");
    delay(250);     
  } 
    if (button4State == LOW) {
    Serial.println("4");
    delay(250);     
  } 

}
