// Use if you want to force the software SPI subsystem to be used for some reason (generally, you don't)
// #define FASTLED_FORCE_SOFTWARE_SPI
// Use if you want to force non-accelerated pin access (hint: you really don't, it breaks lots of things)
// #define FASTLED_FORCE_SOFTWARE_SPI
// #define FASTLED_FORCE_SOFTWARE_PINS
#include <FastLED.h>

///////////////////////////////////////////////////////////////////////////////////////////
//
// Move a white dot along the strip of leds.  This program simply shows how to configure the leds,
// and then how to turn a single pixel white and then off, moving down the line of pixels.
// 

// How many leds are in the strip?
#define NUM_LEDS 300

#define DATA_PIN 5
#define CLOCK_PIN 13

CRGB leds[NUM_LEDS];

//CRGB stripe[] = {CRGB(0,0,0),CRGB(20,0,0),CRGB(50,0,0),CRGB(100,0,0), CRGB(150,0,0), CRGB(200,0,0), CRGB(255,0,0), CRGB::White};
CRGB stripe[] = {CRGB::Green, CRGB::Green,CRGB::White, CRGB::White,CRGB::Green, CRGB::Green,CRGB::White, CRGB::White,CRGB::Green, CRGB::Green,CRGB::White, CRGB::White};

int stripeSize = sizeof(stripe)/sizeof(stripe[0]);

int numStripes = 8;
int delayNum = 15;
CRGB background = CRGB::Red;

// This function sets up the ledsand tells the controller about them
void setup() {
	// sanity check delay - allows reprogramming if accidently blowing power w/leds
   	delay(2000);

    // Uncomment/edit one of the following lines for your leds arrangement.
    // ## Clockless types ##
    // FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
    // FastLED.addLeds<SM16703, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<TM1829, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<TM1812, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<TM1809, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<TM1804, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<TM1803, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<UCS1903, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<UCS1903B, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<UCS1904, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<UCS2903, DATA_PIN, RGB>(leds, NUM_LEDS);
     FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<WS2852, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<GS1903, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<SK6812, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<SK6822, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<APA106, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<PL9823, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<SK6822, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<WS2813, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<APA104, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<WS2811_400, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<GE8822, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<GW6205, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<GW6205_400, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<LPD1886, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<LPD1886_8BIT, DATA_PIN, RGB>(leds, NUM_LEDS);
    // ## Clocked (SPI) types ##
    // FastLED.addLeds<LPD6803, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<WS2803, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<SM16716, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical
    // FastLED.addLeds<DOTSTAR, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical
    // FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical
    // FastLED.addLeds<SK9822, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical
    setAll(background);
    start();
}

// This function runs over and over, and is where you do the magic to light
// your leds.
/*void loop() {
   // Move a single white led 
   for(int i = 0; i < NUM_LEDS; i = i + 1) {
      // Turn our current led on to white, then show the leds
      // leds[whiteLed] = CRGB::White;
      for(int j = i; j < i + 3; j = j + 1){
        leds[i + j] = stripe[j];
      }

      // Show the leds (only one of which is set to white, from above)
      FastLED.show();

      // Wait a little bit
      delay(1);

      // Turn our current led back to black for the next loop around
      leds[i] = CRGB::Black;
   }
}*/


/*int i = 0;
void loop(){
  if(i >= NUM_LEDS-(stripeSize-1)){
    i = 0;
    for(int j = 1; j < stripeSize; ++j){
      leds[NUM_LEDS-j] = background;
    }
    //leds[NUM_LEDS-1] = CRGB::Black;
    //leds[NUM_LEDS-2] = CRGB::Black;
  }
  
  for(int j = 0; j < stripeSize; ++j){
    leds[i + j] = stripe[j];
  }
  FastLED.show();

      // Wait a little bit
  delay(10);

      // Turn our current led back to black for the next loop around
  leds[i] = background;
  ++i;
}*/

void loop(){
  CRGB last = leds[NUM_LEDS-1];
  for(int i = NUM_LEDS-1; i > 0; i -= 1){
      leds[i] = leds[i-1];
  }
  leds[0] = last;
  FastLED.show();
  delay(delayNum);
}

void setAll(CRGB color){
  for(int j = 0; j < NUM_LEDS; ++j){
    leds[j] = color;
  }
  FastLED.show();
}

void start(){
  for(int j = 0; j < numStripes; ++j){
    for(int k = 0; k < stripeSize; ++k){
      leds[((NUM_LEDS/numStripes)*j)+k] = stripe[k];
    }
  }
  FastLED.show();
}
