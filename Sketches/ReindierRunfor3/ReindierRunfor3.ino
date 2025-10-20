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
#define NUM_LEDS 1800

// These define the start and end pixels of the legs.  R1, R2, R3 = which reindeer - R1 is the front.  
//"FR" = front right leg. "FL" = front left leg.  "RR" = rear right, etc.  
//"SRT" = starting pixel.  "END" = ending pixel
#define R1_FR_SRT 1381
#define R1_FR_END 1445
#define R1_FL_SRT 1446
#define R1_FL_END 1491
#define R1_RR_SRT 1515
#define R1_RR_END 1552
#define R1_RL_SRT 1553
#define R1_RL_END 1601
//
#define R2_FR_SRT 776
#define R2_FR_END 844
#define R2_FL_SRT 844
#define R2_FL_END 895
#define R2_RR_SRT 915
#define R2_RR_END 954
#define R2_RL_SRT 954
#define R2_RL_END 1004
//
#define R3_FR_SRT 264
#define R3_FR_END 325
#define R3_FL_SRT 334
#define R3_FL_END 380
#define R3_RR_SRT 410
#define R3_RR_END 449
#define R3_RL_SRT 449
#define R3_RL_END 500


// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 5
#define CLOCK_PIN 13

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];
CRGB baseColor = CRGB::White;

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
    // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<WS2852, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<GS1903, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<SK6812, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<SK6822, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<APA106, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<PL9823, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<SK6822, DATA_PIN, RGB>(leds, NUM_LEDS);
    FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
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
}


// This function runs over and over, and is where you do the magic to light
// your leds.
void loop() {
  

   fill_solid( leds, NUM_LEDS, baseColor);

//This section is for Reindeer #1
   for(int i = R1_FR_SRT; i < R1_FR_END; i++) {
      // Turn off the front right leg of Reindeer #1
      leds[i] = CRGB::Black;
   }
   for(int whiteLed = R1_RL_SRT; whiteLed < R1_RL_END; whiteLed = whiteLed + 1) {
      // Turn off the rear left leg
      leds[whiteLed] = CRGB::Black;
   }  
   for(int whiteLed = R1_FL_SRT; whiteLed < R1_FL_END; whiteLed = whiteLed + 1) {
      // Turn off the front left leg
      leds[whiteLed] = baseColor;
   }
   for(int whiteLed = R1_RR_SRT; whiteLed < R1_RR_END; whiteLed = whiteLed + 1) {
      // Turn off the rear right leg
      leds[whiteLed] = baseColor;
   }

//This section is for Reindeer #2
   for(int whiteLed = R2_FR_SRT; whiteLed < R2_FR_END; whiteLed = whiteLed + 1) {
      // Turn off the front right leg of Reindeer #1
      leds[whiteLed] = CRGB::Black;
   }
   for(int whiteLed = R2_RL_SRT; whiteLed < R2_RL_END; whiteLed = whiteLed + 1) {
      // Turn off the rear left leg
      leds[whiteLed] = CRGB::Black;
   }  
   for(int whiteLed = R2_FL_SRT; whiteLed < R2_FL_END; whiteLed = whiteLed + 1) {
      // Turn off the front left leg
      leds[whiteLed] = CRGB::White;
   }
   for(int whiteLed = R2_RR_SRT; whiteLed < R2_RR_END; whiteLed = whiteLed + 1) {
      // Turn off the rear right leg
      leds[whiteLed] = CRGB::White;
   }

//This section is for Reindeer #3
   for(int whiteLed = R3_FR_SRT; whiteLed < R3_FR_END; whiteLed = whiteLed + 1) {
      // Turn off the front right leg of Reindeer #1
      leds[whiteLed] = CRGB::Black;
   }
   for(int whiteLed = R3_RL_SRT; whiteLed < R3_RL_END; whiteLed = whiteLed + 1) {
      // Turn off the rear left leg
      leds[whiteLed] = CRGB::Black;
   }  
   for(int whiteLed = R3_FL_SRT; whiteLed < R3_FL_END; whiteLed = whiteLed + 1) {
      // Turn off the front left leg
      leds[whiteLed] = CRGB::White;
   }
   for(int whiteLed = R3_RR_SRT; whiteLed < R3_RR_END; whiteLed = whiteLed + 1) {
      // Turn off the rear right leg
      leds[whiteLed] = CRGB::White;
   }
//Keep unused LEDs off
      // Begining of reindeer 3
      for(int whiteLed = 0; whiteLed < 76; whiteLed = whiteLed + 1) {
      leds[whiteLed] = CRGB::Black;
   }
      // End of reindeer 3
      for(int whiteLed = 514; whiteLed < 600; whiteLed = whiteLed + 1) {
      leds[whiteLed] = CRGB::Black; 
   }
      // End of reindeer 2
      for(int whiteLed = 1015; whiteLed < 1200; whiteLed = whiteLed + 1) {
      leds[whiteLed] = CRGB::Black; 
   }
      // End of reindeer 1
      for(int whiteLed = 1611; whiteLed < 1800; whiteLed = whiteLed + 1) {
      leds[whiteLed] = CRGB::Black; 
   } 

//This lights reindeer #1 Nose Red
   for(int whiteLed = 1342; whiteLed < 1348; whiteLed = whiteLed + 1) {
      // Turn our current led on to white, then show the leds
      leds[whiteLed] = CRGB::Green;
   }
    
   FastLED.show();

//set this delay for how fast you want the reindeer to run
  delay(800);

//This section is for Reindeer #1
   for(int whiteLed = R1_FR_SRT; whiteLed < R1_FR_END; whiteLed = whiteLed + 1) {
      // Turn ON the front right leg
      leds[whiteLed] = CRGB::White;
   }
   for(int whiteLed = R1_RL_SRT; whiteLed < R1_RL_END; whiteLed = whiteLed + 1) {
      // Turn ON the rear left leg
      leds[whiteLed] = CRGB::White;
   }
   for(int whiteLed = R1_FL_SRT; whiteLed < R1_FL_END; whiteLed = whiteLed + 1) {
      // Turn off the front left leg
      leds[whiteLed] = CRGB::Black;
   }
   for(int whiteLed = R1_RR_SRT; whiteLed < R1_RR_END; whiteLed = whiteLed + 1) {
      // Turn off the rear right leg
      leds[whiteLed] = CRGB::Black;
   }
   
//This section is for Reindeer #2
   for(int whiteLed = R2_FR_SRT; whiteLed < R2_FR_END; whiteLed = whiteLed + 1) {
      // Turn ON the front right leg
      leds[whiteLed] = CRGB::White;
   }
   for(int whiteLed = R2_RL_SRT; whiteLed < R2_RL_END; whiteLed = whiteLed + 1) {
      // Turn ON the rear left leg
      leds[whiteLed] = CRGB::White;
   }
   for(int whiteLed = R2_FL_SRT; whiteLed < R2_FL_END; whiteLed = whiteLed + 1) {
      // Turn off the front left leg
      leds[whiteLed] = CRGB::Black;
   }
   for(int whiteLed = R2_RR_SRT; whiteLed < R2_RR_END; whiteLed = whiteLed + 1) {
      // Turn off the rear right leg
      leds[whiteLed] = CRGB::Black;
   }

//This section is for Reindeer #3
   for(int whiteLed = R3_FR_SRT; whiteLed < R3_FR_END; whiteLed = whiteLed + 1) {
      // Turn ON the front right leg
      leds[whiteLed] = CRGB::White;
   }
   for(int whiteLed = R3_RL_SRT; whiteLed < R3_RL_END; whiteLed = whiteLed + 1) {
      // Turn ON the rear left leg
      leds[whiteLed] = CRGB::White;
   }
   for(int whiteLed = R3_FL_SRT; whiteLed < R3_FL_END; whiteLed = whiteLed + 1) {
      // Turn off the front left leg
      leds[whiteLed] = CRGB::Black;
   }
   for(int whiteLed = R3_RR_SRT; whiteLed < R3_RR_END; whiteLed = whiteLed + 1) {
      // Turn off the rear right leg
      leds[whiteLed] = CRGB::Black;
   }

//This lights reindeer #1 Nose white
   for(int whiteLed = 1342; whiteLed < 1348; whiteLed = whiteLed + 1) {
      // Turn our current led on to white, then show the leds
      leds[whiteLed] = CRGB::White;
   }
   FastLED.show();

  delay(800);

//This lights reindeer #1 Nose Red
//   for(int whiteLed = 1344; whiteLed < 1348; whiteLed = whiteLed + 1) {
      // Turn our current led on to white, then show the leds
//      leds[whiteLed] = CRGB::Green;

      // Show the leds (only one of which is set to white, from above)
//      FastLED.show();

      // Wait a little bit
//      delay(100);
      // Turn our current led back to black for the next loop around
//      leds[whiteLed] = CRGB::White;
//   }
}
