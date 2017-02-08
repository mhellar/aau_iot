/*
  Analog input, analog output, serial output

 Reads an analog input pin, maps the result to a range from 0 to 255
 and uses the result to set the pulsewidth modulation (PWM) of an output pin.
 Also prints the results to the serial monitor.

 The circuit:
 * potentiometer connected to analog pin 0.
   Center pin of the potentiometer goes to the analog pin.
   side pins of the potentiometer go to +5V and ground
 * LED connected from digital pin 9 to ground

 created 29 Dec. 2008
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPinx = A0;  // Analog input pin that the potentiometer is attached to
const int analogInPiny = A5;  // Analog input pin that the potentiometer is attached to
const int triggerPin = 7;  // Digital input pin that the button is attached to

int sensorValuex = 0;        // value read from the pot x
int sensorValuey = 0;        // value read from the pot y
int trigger = 0;        // value read from the pot y

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(115200);
  pinMode(7, INPUT_PULLUP);
}

void loop() {
  // read the analog in value:
  sensorValuex = analogRead(analogInPinx);
  delay(10);
  sensorValuey = analogRead(analogInPiny);
  trigger = digitalRead(triggerPin);

  // print the results to the serial monitor:
  Serial.print("xval,");
  Serial.print(sensorValuex);
  Serial.print(",yval,");
  Serial.print(sensorValuey);
  Serial.print(",trigger,");
  Serial.println(trigger);
  
  // wait 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(10);
}
