#include <Adafruit_NeoPixel.h>
//pins
#define neoPin 10
#define powerPin A2
#define buttonPinA 13 //color
#define buttonPinB 12 //brightness
//colors and brightness
#define LEDS 10 
int colors[14][3] = { //the color values
                        {0  ,0  ,0  }, //white
                        {255,0  ,0  }, //red
                        {255,125,0  }, //etc.
                        {255,255,0  },
                        {125,255,0  },
                        {0  ,255,0  }, //green
                        {0  ,255,125},
                        {0  ,255,255},
                        {0  ,125,255},
                        {0  ,0  ,255}, //blue
                        {125,0  ,255},
                        {255,0  ,255},
                        {255,0  ,125},
                        {255,255,255},
};
int colorMode = 0; //0 is color cycle, 1-13 is solid colors
int brightness[] = {60,120,255};
int brightnessMode = 0;
//timekeeping
float duration = 15; //in minutes, duration of every cycle
float elapsed; //last timecheck
//buttons
boolean toggleA; //toggles to prevent accidental pushes
boolean toggleB;

//neopixel
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDS, neoPin, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  //powerPin
  pinMode(powerPin,OUTPUT);
  digitalWrite(powerPin,HIGH);
  //buttons
  pinMode(buttonPinA,INPUT);
  pinMode(buttonPinB,INPUT);
  //timekeeping
  elapsed = millis();
  duration = duration*1000*60; //into milliseconds
  //neopixel
  strip.begin();
};

void loop() {
  //update
  update(colorMode);
  //timekeeping
  if(millis() > duration + elapsed){
    elapsed = millis();
    Serial.println("one");
  };
  if(millis() < elapsed){
    elapsed = millis();
    Serial.println("two");
  };
  //buttons
  //A
  if (digitalRead(buttonPinA) == HIGH && toggleA == false){
    toggleA = true;
    brightnessMode++;
    if (brightnessMode > 2){
      brightnessMode = 0;
    };
  };
  if (digitalRead(buttonPinA) == LOW && toggleA == true){
    toggleA = false;
   };
  //B
  if (digitalRead(buttonPinB) == HIGH && toggleB == false){
    toggleB = true;
    colorMode++;
    if (colorMode > 13){
      colorMode = 0;
      elapsed = millis();
    };
  };
  if (digitalRead(buttonPinB) == LOW && toggleB == true){
    toggleB = false;
  };
};

void colorMap(int x){ //up to 1530
  //temporary
  int col_r = 0;
  int col_g = 0;
  int col_b = 0;
  //
  if(x >= 0){
    col_r = 255;
    col_g = 0;
    col_b = x;
  };
  if(x > 255){
    col_r = 255-(x-255);
    col_g = 0;
    col_b = 255;
  };
  if(x > 510){
    col_r = 0;
    col_g = x-510;
    col_b = 255;
  };
  if(x > 765){
    col_r = 0;
    col_g = 255;
    col_b = 255-(x-765);
  };
  if(x > 1020){
    col_r = x-1020;
    col_g = 255;
    col_b = 0;
  };
  if(x > 1275){
    col_r = 255;
    col_g = 255-(x-1275);
    col_b = 0;
  };
  colors[0][0] = col_r;
  colors[0][1] = col_g;
  colors[0][2] = col_b;
};
//updates everythin based on colormapping and time
void update(int y){
  float temp = map(millis()-elapsed, 0, duration, 0, 1530);
  colorMap(temp);
  for(int i = 0; i < LEDS; i++){
    strip.setPixelColor(i,colors[y][0],colors[y][1],colors[y][2]);
  };
  strip.setBrightness(brightness[brightnessMode]);
  strip.show();
};
