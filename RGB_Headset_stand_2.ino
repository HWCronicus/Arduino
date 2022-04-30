#include <Adafruit_NeoPixel.h>

#define BUTTON_PIN   2    // Digital IO pin connected to the button.  This will be
                          // driven with a pull-up resistor so the switch should
                          // pull the pin to ground momentarily.  On a high -> low
                          // transition the button press logic will execute.

#define PIXEL_PIN    6    // Digital IO pin connected to the NeoPixels.

#define PIXEL_COUNT 11     // Number of LEDs in Strip

#define TOUCH_SENSOR 15    // Digital IO pin connected to the touch capcitive sensor

// Parameter 1 = number of pixels in strip,  neopixel stick has 8
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

bool oldState = HIGH;
bool newState = HIGH;
bool oldStateBr = HIGH;
bool newStateBr = HIGH;
int InLoopPress = 0;
int showType = 0;
int br = 1; // Initialize brightness to 100%

// Main Setup
void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode (TOUCH_SENSOR, INPUT_PULLUP);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(9600);
}

// Main Loop
void loop() {
  // Start Effect
  startShow(showType);
}

// Checks if button was pressed and changes variable to go to next effect
void checkInLoopPress() {
    newState = digitalRead(TOUCH_SENSOR);
    if (newState == LOW && oldState == HIGH) {
      InLoopPress = 1;
      showType++;
      // Change this according to the number of effects
      if (showType > 15)
        showType=0;
     //need this below?
     oldState = newState;
    }
}

//Check for brightness button press
void checkBrightnessPress(){
   newStateBr = digitalRead(BUTTON_PIN);
        if (newStateBr == LOW && oldStateBr == HIGH) {
          br++;
          if (br > 7) {
            br = 1;
          }
        }
        oldStateBr = newStateBr;
}

// Selects which effect to start
void startShow(int i) {
  switch(i){
    case 0: rainbow(10);
            break;
    case 1: rainbowCycle(10);
            break;
    case 2: red(10);
            break;
    case 3: green(10);
            break;
    case 4: blue(10);
            break;
    case 5: purple(10);
            break;
    case 6: cyan(10);
            break;
    case 7: RunningLights(255,0,0, 50);            // red
            break;
    case 8: RunningLights(255,255,255, 50);        // white
            break;
    case 9: RunningLights(128,0,128, 50);          // purple
            break;
    case 10: FadeInOut(255, 0, 0); // Red
            break;
    case 11: FadeInOut(0, 255, 0); // green
            break;
    case 12: FadeInOut(0, 0, 255); // blue
            break;
    case 13: FadeInOut(128, 0, 128); // purple
            break;
    case 14: FadeInOut(0, 255, 255); // cyan
            break;
    case 15: FadeInOut(255, 255, 255); // white
            break;
  }
}

// EFFECTS //

void red(uint8_t wait) {
  uint16_t i;
  
  while(1){
        
        for(i=0; i<strip.numPixels(); i++) {
            strip.setPixelColor(i, strip.Color(255/br, 0/br, 0/br)); // red
          }
    
        strip.show();
        delay(wait);
    
        // Check for inloop button press
          oldState = newState;
          checkInLoopPress();
          if (InLoopPress == 1) {
              InLoopPress = 0;
              break;
          }

          checkBrightnessPress();
        
    }
}

void green(uint8_t wait) {
  uint16_t i;

  while(1){

      for(i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(0, 255/br, 0)); // green
        }
  
      strip.show();
      delay(wait);
  
      // Check for inloop button press
        oldState = newState;
        checkInLoopPress();
        if (InLoopPress == 1) {
            InLoopPress = 0;
            break;
        }

        checkBrightnessPress();
    }
}

void blue(uint8_t wait) {
  uint16_t i;

  while(1){

        for(i=0; i<strip.numPixels(); i++) {
            strip.setPixelColor(i, strip.Color(0, 0, 255/br)); // blue
          }
    
        strip.show();
        delay(wait);
    
        // Check for inloop button press
          oldState = newState;
          checkInLoopPress();
          if (InLoopPress == 1) {
              InLoopPress = 0;
              break;
          }

          checkBrightnessPress();
    }
}

void purple(uint8_t wait) {
  uint16_t i;

  while(1){
    
      for(i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(128/br, 0, 128/br)); // purple
        }
  
      strip.show();
      delay(wait);
  
      // Check for inloop button press
        oldState = newState;
        checkInLoopPress();
        if (InLoopPress == 1) {
            InLoopPress = 0;
            break;
        }

        checkBrightnessPress();

    }
}

void cyan(uint8_t wait) {
  uint16_t i;

  while(1){
    
      for(i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(0, 255/br, 255/br)); // cyan
        }
  
      strip.show();
      delay(wait);
  
      // Check for inloop button press
        oldState = newState;
        checkInLoopPress();
        if (InLoopPress == 1) {
            InLoopPress = 0;
            break;
        }

        checkBrightnessPress();

    }
}

void RunningLights(byte red, byte green, byte blue, int WaveDelay) {
  int Position=0;
  int breakLoop=0;

    while(1) {
      for(int j=0; j<strip.numPixels()*2; j++)
      {
          Position++; // = 0; //Position + Rate;
          for(int i=0; i<strip.numPixels(); i++) {
            // sine wave, 3 offset waves make a rainbow!
            //float level = sin(i+Position) * 127 + 128;
            //setPixel(i,level,0,0);
            //float level = sin(i+Position) * 127 + 128;
            strip.setPixelColor(i,((sin(i+Position) * 127 + 128)/255)*(red/br),
                       ((sin(i+Position) * 127 + 128)/255)*(green/br),
                       ((sin(i+Position) * 127 + 128)/255)*(blue/br));
          }
          strip.show();
          delay(WaveDelay);
          checkBrightnessPress();

          // Check for inloop button press
          oldState = newState;
          checkInLoopPress();
          if (InLoopPress == 1) {
              InLoopPress = 0;
              breakLoop = 1;
              break;
          }
    
       }

       if (breakLoop == 1) {
        breakLoop = 0;
        break;
       }
        
    }

}

void FadeInOut(byte red, byte green, byte blue){
  float r, g, b;
  int breakLoop=0;

  while(1) {
      for(int k = 0; k < 256; k=k+1) {
        r = (k/256.0)*red;
        g = (k/256.0)*green;
        b = (k/256.0)*blue;
          for(int i=0; i<strip.numPixels(); i++) {
           strip.setPixelColor(i,r/br,g/br,b/br);
          }
        strip.show();
        delay(10);
        checkBrightnessPress();
        // Check for inloop button press
        oldState = newState;
        checkInLoopPress();
        if (InLoopPress == 1) {
            InLoopPress = 0;
            breakLoop = 1;
            break;
        }
      }

      if (breakLoop == 1) {
        breakLoop = 0;
        break;
      }
         
      for(int k = 255; k >= 0; k=k-2) {
        r = (k/256.0)*red;
        g = (k/256.0)*green;
        b = (k/256.0)*blue;
          for(int i=0; i<strip.numPixels(); i++) {
           strip.setPixelColor(i,r/br,g/br,b/br);
          }
        strip.show();
        delay(10);
        checkBrightnessPress();
        // Check for inloop button press
        oldState = newState;
        checkInLoopPress();
        if (InLoopPress == 1) {
            InLoopPress = 0;
            breakLoop = 1;
            break;
        }
      }

      if (breakLoop == 1) {
        breakLoop = 0;
        break;
      }


  }

  
}

void rainbow(uint8_t wait) {
  uint16_t i, j;
 
  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255, br));
    }
    strip.show();
    delay(wait);

    // Check for inloop button press
    oldState = newState;
    checkInLoopPress();
    if (InLoopPress == 1) {
        InLoopPress = 0;
        break;
    }
    checkBrightnessPress();
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*1; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255,br));
    }
    strip.show();
    delay(wait);
 
    // Check for inloop button press
    oldState = newState;
    checkInLoopPress();
    if (InLoopPress == 1) {
        InLoopPress = 0;
        break;
    }
    checkBrightnessPress();
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos, int br) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color((255 - WheelPos * 3)/br, 0, (WheelPos * 3)/br);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, (WheelPos * 3)/br, (255 - WheelPos * 3)/br);
  }
  WheelPos -= 170;
  return strip.Color((WheelPos * 3)/br, (255 - WheelPos * 3)/br, 0);
}
