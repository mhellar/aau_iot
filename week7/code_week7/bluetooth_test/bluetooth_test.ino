// Basic Bluetooth sketch HC-05_01
// Sends "Bluetooth Test" to the serial monitor and the software serial once every second.
//
// Connect the HC-05 module and data over Bluetooth
//
// The HC-05 defaults to commincation mode when first powered on.
// The default baud rate for communication is 9600
 #include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            2

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      12

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


#include <SoftwareSerial.h>
SoftwareSerial BTserial(10, 11); // RX | TX
// Connect the HC-05 TX to Arduino pin 2 RX. 
// Connect the HC-05 RX to Arduino pin 3 TX through a voltage divider.
// 
int s1;
int s2;
int s3;

int r;
int g;
int b;
  
char c = ' ';
 
void setup() 
{
    Serial.begin(9600);
    Serial.println("Enter AT commands:");
 
    // HC-06 default serial speed for communcation mode is 9600
    BTserial.begin(9600);  
    pixels.begin();
}
 
void loop() 
{
    s1 = analogRead(5);
    s2 = analogRead(4);
    s3 = analogRead(3);

    r = map(s1, 0, 1023, 0, 255);
    g = map(s2, 0, 1023, 0, 255);
    b = map(s3, 0, 1023, 0, 255);
    
    BTserial.print("s1,"); 
    BTserial.print(s1); 
    BTserial.print(",s2,"); 
    BTserial.print(s2); 
    BTserial.print(",s3,"); 
    BTserial.println(s3); 
    
    for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(r,g,b)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    }
    delay(30); 
}
