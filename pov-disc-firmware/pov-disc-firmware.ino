// (c) Michael Schoeffler 2017, http://www.mschoeffler.de
#include "FastLED.h"

#define DATA_PIN 3
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 60
#define BRIGHTNESS 96

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(9600);
  Serial.println(sizeof(CRGB::Red) * 60);

  delay(2000); // initial delay of a few seconds is recommended

  //http://www.8bit-era.cz/arduino-timer-interrupts-calculator.html
  TCCR2A = 0; // set entire TCCR2A register to 0
  TCCR2B = 0; // same for TCCR2B
  TCNT2  = 0; // initialize counter value to 0
  // set compare match register for 4347.826086956522 Hz increments
  OCR2A = 114; // = 16000000 / (32 * 4347.826086956522) - 1 (must be <256)
  // turn on CTC mode
  TCCR2B |= (1 << WGM21);
  // Set CS22, CS21 and CS20 bits for 32 prescaler
  TCCR2B |= (0 << CS22) | (1 << CS21) | (1 << CS20);
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);
  sei(); // allow interrupts


  sei();//allow interrupts

  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip); // initializes LED strip
  FastLED.setBrightness(BRIGHTNESS);// global brightness
}

// switches off all LEDs
void showProgramCleanUp(long delayTime) {
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
  delay(delayTime);
}

// switches on all LEDs. Each LED is shown in random color.
// numIterations: indicates how often LEDs are switched on in random colors
// delayTime: indicates for how long LEDs are switched on.
void showProgramRandom(int numIterations, long delayTime) {
  for (int iteration = 0; iteration < numIterations; ++iteration) {
    for (int i = 0; i < NUM_LEDS; ++i) {
      leds[i] = CHSV(random8(),255,255); // hue, saturation, value
    }
    FastLED.show();
    delay(delayTime);
  }
}

// Shifts a single pixel from the start of strip to the end.
// crgb: color of shifted pixel
// delayTime: indicates how long the pixel is shown on each LED
void showProgramShiftSinglePixel(CRGB crgb, long delayTime) {
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = crgb;
    FastLED.show();
    delay(delayTime);
    leds[i] = CRGB::Black;
  }
}

// Shifts multiple pixel from the start of strip to the end. The color of each pixel is randomized.
// delayTime: indicates how long the pixels are shown on each LED
void showProgramShiftMultiPixel(long delayTime) {
  for (int i = 0; i < NUM_LEDS; ++i) { 
    for (int j = i; j > 0; -- j) {
      leds[j] = leds[j-1];
    }
    CRGB newPixel = CHSV(random8(), 255, 255);
    leds[0] = newPixel;
    FastLED.show();
    delay(delayTime);
  }
}

void showProgramFadeWhite(long delayTime) {
  for (int i=0; i<NUM_LEDS; i++) {
    leds[i] = CRGB::White;
  }
  for (int i=0; i<255; i++) {
      FastLED.setBrightness(i);
      FastLED.show();
      delay(delayTime);
  }
  for (int i=254; i>0; i--) {
      FastLED.setBrightness(i);
      FastLED.show();
      delay(delayTime);
  }
}

void showProgramRainbow(long delayTime) {
  for (int i=0; i<255; i++) {
    CRGB color = CHSV(i, 255, 255);
    for (int l=0; l<NUM_LEDS; l++) {
      leds[l] = color;
    }
    FastLED.show();
    delay(delayTime);
  }
  for (int i=254; i>0; i--) {
    CRGB color = CHSV(i, 255, 255);
    for (int l=0; l<NUM_LEDS; l++) {
      leds[l] = color;
    }
    FastLED.show();
    delay(delayTime);
  }
}

//CRGB quads[] = { CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Yellow };
//void showProgramQuads(long delayTime) {
//  for (int i=0; i<4; i++) {
//      for (int l=0;l<NUM_LEDS/2;l++) {
//        leds[l] = quads[i];
//        leds[NUM_LEDS-l] = quads[(i + 2) & 0x03];
//      }
//      FastLED.show();
//      delay(delayTime);
//  }
//}

uint8_t hues180[] = { 0,1,3,4,6,7,8,10,11,13,14,16,17,18,20,21,23,24,26,27,28,30,31,33,34,35,37,38,40,41,42,44,45,47,48,50,51,52,54,55,57,58,60,61,62,64,65,67,68,69,71,72,74,75,76,78,79,81,82,84,85,86,88,89,91,92,94,95,96,98,99,101,102,103,105,106,108,109,110,112,113,115,116,118,119,120,122,123,125,126,128,129,130,132,133,135,136,137,139,140,142,143,144,146,147,149,150,152,153,154,156,157,159,160,162,163,164,166,167,169,170,171,173,174,176,177,178,180,181,183,184,186,187,188,190,191,193,194,196,197,198,200,201,203,204,205,207,208,210,211,212,214,215,217,218,220,221,222,224,225,227,228,230,231,232,234,235,237,238,239,241,242,244,245,246,248,249,251,252,254 };
void showProgramQuads(long delayTime) {
  for (int i=0;i<180;i++) {
    for (int l=0; l<NUM_LEDS/2;l++) {
      leds[l] = CHSV(hues180[i], 255, 255);
      leds[NUM_LEDS-l-1] = CHSV(hues180[(90+i)%180], 255,255);
    }
    FastLED.show();
    delay(2);
  }
}

volatile uint8_t deg = 0;

// main program
void loop() {
//  Serial.println(millis());
//  showProgramCleanUp(500); // clean up
//  showProgramRandom(100, 100); // show "random" program
//  
//  showProgramCleanUp(500); // clean up
//  showProgramRandom(10, 1000); // show "random" program
//  
//  showProgramCleanUp(500); // clean up
//  showProgramShiftSinglePixel(CRGB::Blue, 250); // show "shift single pixel program" with blue pixel
//  
//  showProgramCleanUp(500); // clean up
//  showProgramShiftSinglePixel(CRGB::Maroon, 100); // show "shift single pixel program" with maroon pixel
//  
//  showProgramCleanUp(500); // clean up
//  showProgramShiftMultiPixel(500); // show "shift multi pixel" program
//  
//  showProgramCleanUp(500); // clean up
//  showProgramShiftMultiPixel(50); // show "shift multi pixel" program
//showProgramFadeWhite(30);
//  showProgramRainbow(1);
//  showProgramQuads(80);

    for (int l=0; l<NUM_LEDS/2;l++) {
      leds[l] = CHSV(hues180[deg], 255, 255);
      leds[NUM_LEDS-l-1] = CHSV(hues180[(90+deg)%180], 255,255);
    }
    FastLED.show();

}

volatile uint8_t timer_divider = 0;
ISR(TIMER2_COMPA_vect){
  deg = (deg + 1) % 180;
}
