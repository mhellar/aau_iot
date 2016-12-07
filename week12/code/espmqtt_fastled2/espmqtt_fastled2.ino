// This example uses an Adafruit Huzzah ESP8266
// to connect to an mqtt server.
//


#include <ESP8266WiFi.h>
#include <MQTTClient.h>

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#include "FastLED.h"
FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    4
//#define CLK_PIN   4
#define LED_TYPE    WS2812
#define COLOR_ORDER RGB
#define NUM_LEDS   63
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

CRGB endclr;
CRGB midclr;

// Most of these variables can be changed.
int ranamount = 96;                                           // The higher the number, lowers the chance for a pixel to light up.
uint8_t thisdelay = 50;                                       // Standard delay value.
bool boolcolours = 1;                                         // True = random, False = array of colours (up to 10)
uint8_t numcolours = 3;                                       // Number of colours in the array to display.
unsigned long colours[10] = {0xff0000, 0x00ff00, 0x0000ff, 0xffffff};  // Just assign the first 3 of these selectable colours.
uint8_t maxbar = 1;                                           // Maximum # of pixels to turn on in a row
uint8_t fadeval = 224;                                        // Fade rate
int stepper = 50;


//set wifi settings here
const char *ssid = "wired";
const char *pass = "12!trout";

WiFiClient net;
MQTTClient client;

int seq;

unsigned long lastMillis = 0;

void connect(); // <- predefine connect() for setup()

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  client.begin("ec2-35-161-110-220.us-west-2.compute.amazonaws.com", net);
  connect();

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
  

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("arduino", "try", "try")) {
    Serial.print(".");
  }

 Serial.println("\nconnected!");

  client.subscribe("/test");
  // client.unsubscribe("/example");
}

void loop() {
  client.loop();
  delay(10); // <- fixes some issues with WiFi stability

  if(!client.connected()) {
    connect();
  }
    // send the 'leds' array out to the actual LED strip
  if(seq == 1){
  rainbowWithGlitter();
  }else if(seq == 2){
  confetti();
  }else if(seq == 3){
  sinelon();
  }else if(seq  == 4){
  bpm();
  }else if(seq == 5){
  juggle();
  }else if(seq  == 6){
  easeMe();
  }else if(seq == 7){
  pop_fade();
  }else if(seq  == 8){
  darksky();
  }else if(seq == 9){
  dawn();
  }else if(seq  == 10){
  wavea();
  }else if(seq == 11){
  waveb();
  }else if(seq  == 12){
  rain();
  }else if(seq == 13){
  sun();
  }     
  
    // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
//  FastLED.delay(200/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow

}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  Serial.print("incoming: ");
  Serial.print(topic);
  Serial.print(" - ");
  Serial.print(payload);
  Serial.println();

  int val = payload.toInt();
  seq = val;
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}


void easeMe() {

  static uint8_t easeOutVal = 18;
  static uint8_t easeInVal  = 6;
  static uint8_t lerpVal    = 18;

  easeOutVal = ease8InOutQuad(easeInVal);
  easeInVal++;

  lerpVal = lerp8by8(0, NUM_LEDS, easeOutVal);

  leds[lerpVal] = CRGB::Blue;
  fadeToBlackBy(leds, NUM_LEDS, 16);                     // 8 bit, 1 = slow, 255 = fast
} // loop()

void pop_fade() {
  unsigned long thiscolour;
  if (ranamount > NUM_LEDS) ranamount = NUM_LEDS;              // Make sure we're at least utilizing ALL the LED's.
  int idex = random16(0, ranamount);

  if (idex < NUM_LEDS) {                                      // Only the lowest probability twinkles will do.
    boolcolours ? thiscolour = random(0x6699FF, 0xffffff) : thiscolour =  colours[random16(0, numcolours)];
    int barlen = random16(1, maxbar);
    for (int i = 0; i < barlen; i++)
      if (idex + i < NUM_LEDS) leds[idex + i] = thiscolour;   // Make sure we don't overshoot the array.
  }
  for (int i = 0; i < NUM_LEDS; i++) leds[i].nscale8(fadeval); // Go through the array and reduce each RGB value by a percentage.
} // pop_fade()



void ChangeMe() {
  uint8_t secondHand = (millis() / 1000) % 30;                // Increase this if you want a longer demo.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.


  thisdelay = 350; colours[0] = 0x0099FF; colours[1] = 0x3300CC; colours[3] = 0x33CCFF; colours[4] = 0x000099; numcolours = 5; boolcolours = 0; fadeval = 192; maxbar = 8;

} // ChangeMe()

void weather() {
  uint8_t speed = 1;
  endclr = blend(CRGB::Aqua, CRGB::Aquamarine, speed);
  midclr = blend(CRGB::Aquamarine, CRGB::Aqua, speed);
  cycle();
  FastLED.show();
}
void weather2() {
  uint8_t speed = 1;
  endclr = blend(CRGB::Orange, CRGB::Aqua, speed);
  midclr = blend(CRGB::Aqua, CRGB::Orange, speed);
  cycle();
  FastLED.show();
}


void weather3() {
  uint8_t speed = 1;
  endclr = blend(CRGB::Orange, CRGB::DarkOrange, speed);
  midclr = blend(CRGB::DarkOrange, CRGB::Orange, speed);
  cycle();
  FastLED.show();
}
void cycle() {

  fill_gradient_RGB(leds, 0, endclr, NUM_LEDS / 2, midclr);
  fill_gradient_RGB(leds, NUM_LEDS / 2 + 1, midclr, NUM_LEDS, endclr);
}

void darksky() {
  // Move a single white led
  for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 2 ) {
    // Turn our current led on to white, then show the leds
    leds[whiteLed] = CRGB::DarkBlue;
    leds[whiteLed + 1] = CRGB::DarkBlue;
    // Show the leds (only one of which is set to white, from above)
    FastLED.show();

    // Wait a little bit
    delay(5);

    // Turn our current led back to black for the next loop around
    leds[whiteLed] = CRGB::DeepSkyBlue;
  }
}

void sky() {
  // Move a single white led
  for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 2 ) {
    // Turn our current led on to white, then show the leds
    leds[whiteLed] = CRGB::RoyalBlue;
    leds[whiteLed + 1] = CRGB::RoyalBlue;
    // Show the leds (only one of which is set to white, from above)
    FastLED.show();


    // Turn our current led back to black for the next loop around
    leds[whiteLed] = CRGB::Aqua;
  }
}

void dawn() {
  // Move a single white led
  for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 2 ) {
    // Turn our current led on to white, then show the leds
    leds[whiteLed] = CRGB::Yellow;
    leds[whiteLed + 1] = CRGB::Yellow;
    // Show the leds (only one of which is set to white, from above)
    FastLED.show();

    // Turn our current led back to black for the next loop around
    leds[whiteLed] = CRGB::Orange;
  }
}
void sun() {
  // Move a single white led
  for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 2 ) {
    // Turn our current led on to white, then show the leds
    leds[whiteLed] = CRGB::Orange;
    leds[whiteLed + 1] = CRGB::Orange;
    // Show the leds (only one of which is set to white, from above)
    FastLED.show();


    // Turn our current led back to black for the next loop around
    leds[whiteLed] = CRGB::OrangeRed;
  }
}

void wavea() {
  // Move a single white led
  for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1 ) {
    // Turn our current led on to white, then show the leds
    leds[whiteLed] = CRGB::Blue;
    leds[whiteLed + 1] = CRGB::Blue;
    leds[whiteLed + 2] = CRGB::Blue;
    leds[whiteLed + 3] = CRGB::Blue;

    // Show the leds (only one of which is set to white, from above)
    FastLED.show();

    // Wait a little bit

    // Turn our current led back to black for the next loop around
    leds[whiteLed] = CRGB::Aqua;
  }
}

void waveb() {
  // Move a single white led
  for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1 ) {
    // Turn our current led on to white, then show the leds
    leds[whiteLed] = CRGB::Aqua;
    leds[whiteLed + 1] = CRGB::Aqua;
    leds[whiteLed + 2] = CRGB::Aqua;
    leds[whiteLed + 3] = CRGB::Aqua;

    // Show the leds (only one of which is set to white, from above)
    FastLED.show();

    // Wait a little bit

    // Turn our current led back to black for the next loop around
    leds[whiteLed] = CRGB::Blue;
  }
}

void rain() {
  ChangeMe();                                                 // Muck those variable around.
  EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
    pop_fade();                                                 // Simple call to the routine.
  }
  show_at_max_brightness_for_power();
} // loop()


const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
  CRGB::Orange,
  CRGB::Orange, // 'white' is too bright compared to red and blue
  CRGB::Orange,
  CRGB::Aqua,

  CRGB::Aqua,
  CRGB::Orange,
  CRGB::Orange,
  CRGB::Orange,

  CRGB::Aqua,
  CRGB::Aqua,
  CRGB::Aqua,
  CRGB::Aqua,
  CRGB::Aqua,
  CRGB::Aqua,
  CRGB::Aqua,
  CRGB::Orange


};

