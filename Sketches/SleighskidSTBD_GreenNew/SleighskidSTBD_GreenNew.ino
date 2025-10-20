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
CRGB stripe[] = {CRGB::Red, CRGB::Red,CRGB::White, CRGB::White,CRGB::Red, CRGB::Red,CRGB::White, CRGB::White,CRGB::Red, CRGB::Red,CRGB::White, CRGB::White};

int stripeSize = sizeof(stripe)/sizeof(stripe[0]);

int numStripes = 8;
int delayNum = 15;
CRGB background = CRGB::Green;

const uint8_t maxBrightness = 255;

//Filter Bools
bool flashingOn = false;
bool doubleOn = false;

//Mode Bools
bool defaultOn = false;
bool rainbowOn = false;
bool snakeOn = false;
bool sparkleOn = false;

//Default Variables
unsigned long defaultLastMillis;
unsigned long defaultIntervalTime = 15;
bool white = false;

//Rainbow Variables
uint8_t rainbowValue = 0;
uint8_t rainbowSpeed = 3;

//Snake Variables
int snakeLength = 175;
int snakeSpeed = 0;
int snakePos = 0;

//Sparkle Variables
unsigned long sparkleLastMillis;
unsigned long sparkleIntervalTime = 500;
int sparkleWidth = 0;
CRGB sparkleColorOne = background;
CRGB sparkleColorTwo = CRGB::Black;

//Flash Variables
unsigned long flashLastMillis;
unsigned long flashIntervalTime = 300;
bool flashOn = true;

bool resetFilters = false;

//Button Variables
unsigned long debounceDelay = 50;
unsigned long lastDebounceTime[] = {0,0,0,0,0}; 
const int numButtons = 5;
const int buttonPin[] = {2,3,4,6,8};
int buttonState[] = {LOW, LOW, LOW, LOW, LOW};
int lastButtonState[] = {LOW, LOW, LOW, LOW, LOW};

//Switch Variables
const int numSwitches = 1;
const int switchPin[] = {7};

// This function sets up the ledsand tells the controller about them
void setup() {
	// sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);
  defaultLastMillis = millis();
  sparkleLastMillis = millis();
  flashLastMillis = millis();
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
  for(int i = 0; i < numButtons; i++){
    pinMode(buttonPin[i], INPUT_PULLUP);
  }
  for(int i = 0; i < numSwitches; i++){
    pinMode(switchPin[i], INPUT_PULLUP);
  }
  setAll(background);
  //start();
  delay(10000);
  button(0);
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
  readButtons();

  flashingOn = !digitalRead(switchPin[0]);
  //doubleOn = !digitalRead(switchPin[1]);

//Modes
  if(defaultOn){
    unsigned long currentTime = millis();
    if(currentTime - defaultLastMillis >= defaultIntervalTime){
      defaultLastMillis = currentTime;
      runDefault();
    }
  }else if(rainbowOn){
    rainbow();
  }else if(snakeOn){
    snake();
  }else if(sparkleOn){
    unsigned long currentTime = millis();
    if(currentTime - sparkleLastMillis >= sparkleIntervalTime){
      sparkleLastMillis = currentTime;
      sparkle();
    }
  }
//Filters
  if(flashingOn){
    flash();
    resetFilters = false;
  }
//Filter Reset
  if(!flashingOn && !resetFilters){
    FastLED.setBrightness(maxBrightness);
    FastLED.show();
    resetFilters = true;
  }
}

//Mode Functions

void runDefault(){
  CRGB last = leds[NUM_LEDS-1];
  for(int i = NUM_LEDS-1; i > 0; i -= 1){
      leds[i] = leds[i-1];
  }
  leds[0] = last;
  FastLED.show();
}

void rainbow(){
  fill_rainbow(leds, NUM_LEDS, rainbowValue, 3);
  rainbowValue -= rainbowSpeed;
  FastLED.show();
}

void snake(){
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  for(int i = 0; i < snakeLength; i++){
    int j = snakePos + i;
    if(j >= NUM_LEDS){
      j -= NUM_LEDS;
    }
    float fi = i;
    float fsnakeLength = snakeLength;
    float percent = (fi/fsnakeLength)*(fi/fsnakeLength);
    uint8_t fade = (1-(percent))*255;
    leds[j] = background;
    leds[j].fadeLightBy(fade);
  }
  snakePos += snakeSpeed;
  if(snakePos >= NUM_LEDS){
    snakePos -= NUM_LEDS;
  }
  FastLED.show();
}

void sparkle(){
  bool p = false;
  for(int i = 0; i < NUM_LEDS; i++){
    int mod = i%(sparkleWidth);
    if(mod == 0){
      p = !p;
    }
    if(p){
      leds[i] = sparkleColorOne;
    }else{
      leds[i] = sparkleColorTwo;
    }
  }
  CRGB tempColor = sparkleColorOne;
  sparkleColorOne = sparkleColorTwo;
  sparkleColorTwo = tempColor;
  FastLED.show();
}

//Filter Functions
void flash(){
  unsigned long currentTime = millis();
  if(currentTime - flashLastMillis >= flashIntervalTime){
    flashLastMillis = currentTime;
    if(flashOn){
      flashOn = false;
      FastLED.setBrightness(0);
      FastLED.show();
    }else{
      flashOn = true;
      FastLED.setBrightness(maxBrightness);
      FastLED.show();
    }
  }
}

//Other Stuff

void setAll(CRGB color){
  fill_solid(leds, NUM_LEDS, color);
  FastLED.show();
}

void start(){
  for(int j = 0; j < numStripes; ++j){
    for(int k = 0; k < stripeSize; ++k){
      leds[((NUM_LEDS/numStripes)*j)+k] = CRGB::White;
    }
  }
  FastLED.show();
}

//Buttons
void readButtons(){
  for(int i = 0; i < numButtons; i++){
    int reading = digitalRead(buttonPin[i]);
    if (reading != lastButtonState[i]) {
      lastDebounceTime[i] = millis();
    }
    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (reading != buttonState[i]) {
        buttonState[i] = reading;
        if (buttonState[i] == HIGH && millis()>5000) {
          button(i);
        }
      }
    }
    lastButtonState[i] = reading;
  }
}

void allOff(){
  defaultOn = false;
  rainbowOn = false;
  snakeOn = false;
  sparkleOn = false;
}

void button(int buttonNum){
  if(buttonNum == 0){
    allOff();
    white = !white;
    defaultOn = true;
    if(white){
      fill_solid(leds, NUM_LEDS, background);
    }else{
      fill_solid(leds, NUM_LEDS, CRGB::Blue);
    }
    start();
  }else if(buttonNum == 1){
    allOff();
    white = false;
    rainbowOn = true;
  }else if(buttonNum == 2){
    allOff();
    white = false;
    defaultOn = true;
    fill_solid(leds, NUM_LEDS, background);
    start();
  }else if(buttonNum == 3){
    allOff();
    white = false;
    snakeOn = true;
    snakeSpeed += 3;
    if(snakeSpeed > 18){
      snakeSpeed = 3;
    }
  }else if(buttonNum == 4){
    allOff();
    white = false;
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    sparkleOn = true;
    sparkleWidth += 1;
    if(sparkleWidth >= 10){
      sparkleWidth = 1;
    }
  }
}