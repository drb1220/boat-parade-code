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

#define R1_START 1200
#define R1_END 1611
//
#define R2_FR_SRT 776
#define R2_FR_END 844
#define R2_FL_SRT 844
#define R2_FL_END 895
#define R2_RR_SRT 915
#define R2_RR_END 954
#define R2_RL_SRT 954
#define R2_RL_END 1004

#define R2_START 600
#define R2_END 1015
//
#define R3_FR_SRT 264
#define R3_FR_END 325
#define R3_FL_SRT 334
#define R3_FL_END 380
#define R3_RR_SRT 410
#define R3_RR_END 449
#define R3_RL_SRT 449
#define R3_RL_END 500

#define R3_START 76
#define R3_END 514


// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 5
#define CLOCK_PIN 13

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];
CRGB colorArray[] = {CRGB::Red,CRGB::Orange,CRGB::Yellow,CRGB::Green,CRGB::Blue,CRGB::Indigo,CRGB::Violet};
int colorIndex = 0;
CRGB baseColor = CRGB::Red;
CRGB rOneColor = CRGB::Red;
CRGB rTwoColor = CRGB::Green;
CRGB rThreeColor = CRGB::Blue;

const uint8_t maxBrightness = 100;

bool solidOn = false;
bool walkingOn = false;
bool rainbowOn = false;
bool fadingOn = false;
bool flashingOn = false;
bool snakeModeOn = false;
bool snakeModeFasterOn = false;
bool snakeModeFadedOn = false;
bool alternateOn = false;
bool sparkleOn = false;





bool started = false;


//Rainbow Variables
uint8_t rainbowValue = 0;
uint8_t rainbowSpeed = 20;

//Solid Variables
bool white = false;
bool chasing = false;

//Walking Variables
unsigned long stepLastMillis;
unsigned long stepIntervalTime = 800;
bool step = true;

//Snake Variables
int snakeLength = 150;
int solidSnakeLength = 100;
int snakeSpeed = 0;
int rOnePos = R1_START;
int rTwoPos = R2_START;
int rThreePos = R3_START;
bool doubleOn = false;
//CRGB colorOne = CGRB::Green;
//CRGB colorTwo = CRGB::Red;

//Fade Variables
uint8_t brightness = maxBrightness;
bool fadeUp = false;
uint8_t fadeSpeed = 5;

//Flash Variables
unsigned long flashLastMillis;
unsigned long flashIntervalTime = 300;
bool flashOn = true;

//Alternate Variables
unsigned long alternateLastMillis;
unsigned long alternateIntervalTime = 800;
bool alternateStep = false;

//Sparkle Variables
unsigned long sparkleLastMillis;
unsigned long sparkleIntervalTime = 500;
bool sparkleMode = false;
int sparkleWidth = 0;
int sparkleDensity = 5;
uint8_t sparkleFadeValue = 100;
CRGB sparkleColorOne = CRGB::White;
CRGB sparkleColorTwo = CRGB::Black;

//AutoLights Variables
unsigned long autoLastMillis;
unsigned long autoIntervalTime = 300000;
uint8_t autoArray[] = {0,0,1,2,3,4};
uint8_t autoIndex = 0;

bool resetFilters = false;

//Button Variables
unsigned long debounceDelay = 50;
unsigned long lastDebounceTime[] = {0,0,0,0,0}; 
const int numButtons = 5;
const int buttonPin[] = {2,3,4,6,9};
int buttonState[] = {LOW, LOW, LOW, LOW, LOW};
int lastButtonState[] = {LOW, LOW, LOW, LOW, LOW};

//Switch Variables
const int numSwitches = 2;
const int switchPin[] = {7,8};


// This function sets up the ledsand tells the controller about them
void setup() {
	// sanity check delay - allows reprogramming if accidently blowing power w/leds
   	delay(2000);
    FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);
    
    for(int i = 0; i < numButtons; i++){
      pinMode(buttonPin[i], INPUT_PULLUP);
    }
    for(int i = 0; i < numSwitches; i++){
      pinMode(switchPin[i], INPUT_PULLUP);
    }
    randomSeed(analogRead(0));
    delay(10000);
    stepLastMillis = millis();
    flashLastMillis = millis();
    sparkleLastMillis = millis();
    autoLastMillis = millis();
    button(0);
}

// This function runs over and over, and is where you do the magic to light
// your leds.
void loop() {
  //startColor();
//Inputs
  readButtons();
  autoLights();

  flashingOn = !digitalRead(switchPin[0]);
  doubleOn = !digitalRead(switchPin[1]);

//Legs
  if(walkingOn){
    if(step){
      unsigned long currentTime = millis();
      if(currentTime - stepLastMillis >= stepIntervalTime){
        stepLastMillis = currentTime;
        fill_solid(leds, NUM_LEDS, baseColor);
        unusedOff();
        runStepOne();
        colorCorrection();
        FastLED.show();
      }
    }else{
      if(millis() - stepLastMillis >= stepIntervalTime){
        stepLastMillis += stepIntervalTime;
        fill_solid(leds, NUM_LEDS, baseColor);
        unusedOff();
        runStepTwo();
        colorCorrection();
        FastLED.show();
      }
    }
  }
//Modes
  if(solidOn){
    if(doubleOn){
      chasing = true;
      walkingOn = false;
      snakeDouble();
    }else if(chasing){
      chasing = false;
      walkingOn = true;
      fillColor();
    }
  }else if(rainbowOn){
    rainbow();
  }else if(snakeModeOn){
    snakeMode();
  }else if(snakeModeFasterOn){
    //snakeModeFaster();
    //snakeDouble();
  }else if(snakeModeFadedOn){
    if(doubleOn){
      snakeDouble();
    }else{
      snakeModeFaded();
    }
  }else if(alternateOn){
    unsigned long currentTime = millis();
    if(currentTime - alternateLastMillis >= alternateIntervalTime){
      alternateLastMillis = currentTime;
      alternate();
    }
  }else if(sparkleOn){
    unsigned long currentTime = millis();
    if(currentTime - sparkleLastMillis >= sparkleIntervalTime){
      sparkleLastMillis = currentTime;
      sparkle();
    }
  }

//Filters
  if(fadingOn){
    fade();
    resetFilters = false;
  }
  if(flashingOn){
    flash();
    resetFilters = false;
  }
//Filter Reset
  if(!fadingOn && !flashingOn && !resetFilters){
    FastLED.setBrightness(maxBrightness);
    FastLED.show();
    resetFilters = true;
  }
  /*if(sparkleOn){
    if(millis() - sparkleLastMillis >= sparkleIntervalTime){
      sparkleLastMillis += sparkleIntervalTime;
      sparkle();
    }
  }*/
}

//Steps

void runStepOne(){
//This section is for Reindeer #1
  step = false;
  for(int i = R1_FR_SRT; i < R1_FR_END; i++) {
    // Turn off the front right leg of Reindeer #1
     leds[i] = CRGB::Black;
  }
  for(int i = R1_RL_SRT; i < R1_RL_END; i++) {
    // Turn off the rear left leg
    leds[i] = CRGB::Black;
  }  
   for(int i = R1_FL_SRT; i < R1_FL_END; i++) {
    // Turn on the front left leg
    leds[i] = baseColor;
  }
   for(int i = R1_RR_SRT; i < R1_RR_END; i++) {
    // Turn on the rear right leg
    leds[i] = baseColor;
  }
//This section is for Reindeer #2
  for(int i = R2_FR_SRT; i < R2_FR_END; i++) {
    // Turn off the front right leg of Reindeer #1
     leds[i] = CRGB::Black;
  }
  for(int i = R2_RL_SRT; i < R2_RL_END; i++) {
    // Turn off the rear left leg
    leds[i] = CRGB::Black;
  }  
   for(int i = R2_FL_SRT; i < R2_FL_END; i++) {
    // Turn on the front left leg
    leds[i] = baseColor;
  }
   for(int i = R2_RR_SRT; i < R2_RR_END; i++) {
    // Turn on the rear right leg
    leds[i] = baseColor;
  }
//This section is for Reindeer #3
  for(int i = R3_FR_SRT; i < R3_FR_END; i++) {
    // Turn off the front right leg of Reindeer #1
    leds[i] = CRGB::Black;
  }
  for(int i = R3_RL_SRT; i < R3_RL_END; i++) {
    // Turn off the rear left leg
    leds[i] = CRGB::Black;
  }  
  for(int i = R3_FL_SRT; i < R3_FL_END; i++) {
    // Turn ON the front left leg
    leds[i] = baseColor;
  }
  for(int i = R3_RR_SRT; i < R3_RR_END; i++) {
    // Turn ON the rear right leg
    leds[i] = baseColor;
  }
//This lights reindeer #1 Nose Red
  for(int i = 1342; i < 1348; i++) {
    leds[i] = CRGB::Red;
  }
}

void runStepTwo(){
//This section is for Reindeer #1
  step = true;
  for(int i = R1_FR_SRT; i < R1_FR_END; i++) {
    // Turn on the front right leg of Reindeer #1
     leds[i] = baseColor;
  }
  for(int i = R1_RL_SRT; i < R1_RL_END; i++) {
    // Turn on the rear left leg
    leds[i] = baseColor;
  }  
   for(int i = R1_FL_SRT; i < R1_FL_END; i++) {
    // Turn off the front left leg
    leds[i] = CRGB::Black;
  }
   for(int i = R1_RR_SRT; i < R1_RR_END; i++) {
    // Turn off the rear right leg
    leds[i] = CRGB::Black;
  }
 //This section is for Reindeer #2
  for(int i = R2_FR_SRT; i < R2_FR_END; i++) {
    // Turn on the front right leg of Reindeer #1
    leds[i] = baseColor;
  }
  for(int i = R2_RL_SRT; i < R2_RL_END; i++) {
    // Turn on the rear left leg
    leds[i] = baseColor;
  }  
   for(int i = R2_FL_SRT; i < R2_FL_END; i++) {
    // Turn off the front left leg
    leds[i] = CRGB::Black;
  }
   for(int i = R2_RR_SRT; i < R2_RR_END; i++) {
    // Turn off the rear right leg
    leds[i] = CRGB::Black;
  }
//This section is for Reindeer #3
  for(int i = R3_FR_SRT; i < R3_FR_END; i++) {
    // Turn ON the front right leg
    leds[i] = baseColor;
  }
  for(int i = R3_RL_SRT; i < R3_RL_END; i++) {
    // Turn ON the rear left leg
    leds[i] = baseColor;
  }
  for(int i = R3_FL_SRT; i < R3_FL_END; i++) {
    // Turn off the front left leg
    leds[i] = CRGB::Black;
  }
  for(int i = R3_RR_SRT; i < R3_RR_END; i++) {
    // Turn off the rear right leg
    leds[i] = CRGB::Black;
  }
//This lights reindeer #1 Nose Base Color
  for(int i = 1342; i < 1348; i++) {
    leds[i] = baseColor;
  }
}

void unusedOff(){
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

}

void colorCorrection(){
  for(int i = 301; i <= 900; i++){
    int r = leds[i].red;
    leds[i].red = leds[i].green;
    leds[i].green = r;
  }
}

void startColor(){
  if(!started){
    delay(1000);
    FastLED.setBrightness(maxBrightness);
    fill_solid(leds, NUM_LEDS, baseColor);
    unusedOff();
    colorCorrection();
    FastLED.show();
    FastLED.setBrightness(maxBrightness);
    FastLED.show();
    started = true;
  }
}

void fillSeparate(){
  //Reindeer 1
  FastLED.setBrightness(maxBrightness);
  for(int i = R1_START; i < R1_END; i++) {
    leds[i] = rOneColor;
  }
  //Reindeer 2
  for(int i = R2_START; i < R2_END; i++) {
    leds[i] = rTwoColor;
  }
  //Reindeer 3
  for(int i = R3_START; i < R3_END; i++) {
    leds[i] = rThreeColor;
  }
}

//Modes

void fillColor(){
  fill_solid(leds, NUM_LEDS, baseColor);
  unusedOff();
  colorCorrection();
  FastLED.show();
}


/*void sparkle(){
  for(int i = 0; i < NUM_LEDS; i++){
    if(random(0,2)==1 && leds[i].operator bool()){
      if(!faded[i]){
        leds[i].fadeLightBy(253);
        faded[i] = true;
      }else{
        leds[i] = baseColor;
        faded[i] = false;
      }
    }
  }
  //colorCorrection();
  FastLED.show();
}*/

void rainbow(){
  fill_rainbow(leds, NUM_LEDS, rainbowValue, 3);
  rainbowValue += rainbowSpeed;
  colorCorrection();
  unusedOff();
  FastLED.show();
}

void snakeMode(){
  //Reindeer 1
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  for(int i = 0; i < snakeLength; i++){
    int j = rOnePos + i;
    if(j >= R1_END){
      j -= (R1_END-R1_START);
    }
    leds[j] = CRGB::White;
  }
  rOnePos += snakeSpeed;
  if(rOnePos >= R1_END){
    rOnePos -= (R1_END-R1_START);
  }

  //Reindeer 2
  for(int i = 0; i < snakeLength; i++){
    int j = rTwoPos + i;
    if(j >= R2_END){
      j -= (R2_END-R2_START);
    }
    leds[j] = CRGB::White;
  }
  rTwoPos += snakeSpeed;
  if(rTwoPos >= R2_END){
    rTwoPos -= (R2_END-R2_START);
  }

  //Reindeer 3
  for(int i = 0; i < snakeLength; i++){
    int j = rThreePos + i;
    if(j >= R3_END){
      j -= (R3_END-R3_START);
    }
    leds[j] = CRGB::White;
  }
  rThreePos += snakeSpeed;
  if(rThreePos >= R3_END){
    rThreePos -= (R3_END-R3_START);
  }
  colorCorrection();
  FastLED.show();
}

void snakeModeFaster(){
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  for(int i = 0; i < solidSnakeLength; i++){
    int j = rOnePos + i;
    if(j >= R1_END){
      j -= (R1_END-R1_START);
    }
    leds[j] = CRGB::White;
    leds[(j-R1_START)+R2_START] = CRGB::White;
    leds[(j-R1_START)+R3_START] = CRGB::White;
  }
  rOnePos += snakeSpeed;
  if(rOnePos >= R1_END){
    rOnePos -= (R1_END-R1_START);
  }
  colorCorrection();
  FastLED.show();
}

void snakeModeFaded(){
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  for(int i = 0; i < snakeLength; i++){
    int j = rOnePos + i;
    if(j >= R1_END){
      j -= (R1_END-R1_START);
    }
    float fi = i;
    float fsnakeLength = snakeLength;
    float percent = (fi/fsnakeLength)*(fi/fsnakeLength);
    uint8_t fade = (1-(percent))*255;
    leds[j] = CRGB::White;
    leds[j].fadeLightBy(fade);
    leds[(j-R1_START)+R2_START] = CRGB::White;
    leds[(j-R1_START)+R2_START].fadeLightBy(fade);
    leds[(j-R1_START)+R3_START] = CRGB::White;
    leds[(j-R1_START)+R3_START].fadeLightBy(fade);
  }
  rOnePos += snakeSpeed;
  if(rOnePos >= R1_END){
    rOnePos -= (R1_END-R1_START);
  }
  colorCorrection();
  FastLED.show();
}

void snakeDouble(){
  CRGB colorOne = CRGB::Green;
  CRGB colorTwo = CRGB::Red;
  if(solidOn){
    colorOne = baseColor;
    colorTwo = baseColor;
  }
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  for(int i = 0; i < snakeLength; i++){
    int j = rOnePos + i;
    if(j >= R1_END){
      j -= (R1_END-R1_START);
    }
    int k = j + (R1_END-R1_START)/2;
    if(k >= R1_END){
      k -= (R1_END-R1_START);
    }
    float fi = i;
    float fsnakeLength = snakeLength;
    float percent = (fi/fsnakeLength)*(fi/fsnakeLength);
    uint8_t fade = (1-(percent))*255;
    leds[j] = colorOne;
    leds[j].fadeLightBy(fade);
    leds[(j-R1_START)+R2_START] = colorOne;
    leds[(j-R1_START)+R2_START].fadeLightBy(fade);
    leds[(j-R1_START)+R3_START] = colorOne;
    leds[(j-R1_START)+R3_START].fadeLightBy(fade);

    leds[k] = colorTwo;
    leds[k].fadeLightBy(fade);
    leds[(k-R1_START)+R2_START] = colorTwo;
    leds[(k-R1_START)+R2_START].fadeLightBy(fade);
    leds[(k-R1_START)+R3_START] = colorTwo;
    leds[(k-R1_START)+R3_START].fadeLightBy(fade);
  }
  rOnePos += snakeSpeed;
  if(rOnePos >= R1_END){
    rOnePos -= (R1_END-R1_START);
  }
  colorCorrection();
  FastLED.show();
}

void alternate(){
  if(alternateStep){
    rOneColor = CRGB::Red;
    rTwoColor = CRGB::Green;
    rThreeColor = CRGB::Red;
  }else{
    rOneColor = CRGB::Green;
    rTwoColor = CRGB::Red;
    rThreeColor = CRGB::Green;
  }
  alternateStep = !alternateStep;
  fillSeparate();
  unusedOff();
  colorCorrection();
  FastLED.show();
}

void sparkle(){
  if(true){
    bool p = false;
    for(int i = R1_START; i <= R1_END; i++){
      int mod = i%(sparkleWidth);
      if(mod == 0){
        p = !p;
      }
      if(p){
        leds[i] = sparkleColorOne;
        leds[(i-R1_START)+R2_START] = sparkleColorOne;
        leds[(i-R1_START)+R3_START] = sparkleColorOne;
      }else{
        leds[i] = sparkleColorTwo;
        leds[(i-R1_START)+R2_START] = sparkleColorTwo;
        leds[(i-R1_START)+R3_START] = sparkleColorTwo;
      }
    }
    CRGB tempColor = sparkleColorOne;
    sparkleColorOne = sparkleColorTwo;
    sparkleColorTwo = tempColor;
    unusedOff();
    colorCorrection();
    FastLED.show();
  }else{
    for(int i = 0; i < sparkleDensity; i++){
      long r = random(R1_START,R1_END);
      if(leds[r].red < 50){
        leds[r] = CRGB::White;
        leds[(i-R1_START)+R2_START] = CRGB::White;
        leds[(i-R1_START)+R3_START] = CRGB::White;
      }else{
        i--;
      }
    }
    for(int i = R1_START; i <= R1_END; i++){
      if(leds[i].red != 0){
        leds[i].fadeToBlackBy(sparkleFadeValue);
        leds[(i-R1_START)+R2_START] = leds[i];
        leds[(i-R1_START)+R3_START] = leds[i];
      }
    }
  }
}

//Filters
void fade(){
  if(!fadeUp){
    if(brightness <= 0){
      brightness = 0;
      fadeUp = true;
    }else{
      brightness -= fadeSpeed;
    }
  }else{
    if(brightness + fadeSpeed >= maxBrightness){
      brightness = maxBrightness;
      fadeUp = false;
    }else{
      brightness += fadeSpeed;
    }
  }
  FastLED.setBrightness(brightness);
  FastLED.show();
}

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
          autoLastMillis = millis();
          autoIndex = 0;
          button(i);
        }
      }
    }
    lastButtonState[i] = reading;
  }
}

void allOff(){
  solidOn = false;
  walkingOn = false;
  rainbowOn = false;
  snakeModeOn = false;
  snakeModeFasterOn = false;
  snakeModeFadedOn = false;
  alternateOn = false;
  sparkleOn = false;
}

void button(int buttonNum){
  FastLED.setBrightness(maxBrightness);
  if(buttonNum == 0){
    allOff();
    solidOn = true;
    walkingOn = true;
    white = !white;
    if(white){
      baseColor = CRGB::White;
    }else{
      baseColor = CRGB::Blue;
    }
    fill_solid(leds, NUM_LEDS, baseColor);
    unusedOff();
    colorCorrection();
    FastLED.show();
  }else if(buttonNum == 1){
    allOff();
    white = false;
    rainbowOn = true;
  }else if(buttonNum == 2){
    allOff();
    white = false;
    alternateOn = true;
    alternateIntervalTime = 400;
    /*alternateIntervalTime -= 100;
    if(alternateIntervalTime < 200){
      alternateIntervalTime = 800;
    }*/
  }else if(buttonNum == 3){
    allOff();
    white = false;
    snakeModeFadedOn = true;
    snakeSpeed = 15;
    /*snakeSpeed += 5;
    if(snakeSpeed > 30){
      snakeSpeed = 5;
    }*/
  }else if(buttonNum == 4){
    allOff();
    white = false;
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    sparkleOn = true;
    sparkleWidth = 5;
    /*sparkleWidth += 1;
    if(sparkleWidth >= 10){
      sparkleWidth = 1;
    }*/
  }
}

void autoLights(){
  unsigned long currentTime = millis();
    if(currentTime - autoLastMillis >= autoIntervalTime){
      autoLastMillis = currentTime;
      button(autoArray[autoIndex]);
      autoIndex++;
      if(autoIndex > 5){
        autoIndex = 0;
      }
    }
}