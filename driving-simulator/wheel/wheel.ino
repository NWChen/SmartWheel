#include <Adafruit_NeoPixel.h>
#include <Servo.h> 
#include <TimerOne.h>
#include "TM1637.h"
#define ON 1
#define OFF 0

// misc and clock constants
int pos = 0;
int8_t TimeDisp[] = {0x00,0x00,0x00,0x00};
unsigned char ClockPoint = 1;
unsigned char Update;
unsigned char halfsecond = 0;
unsigned char second = 0;
unsigned char minute = 0;
unsigned char hour = 0;

// pins
const int LEFTPR = 2;
const int RIGHTPR = 1;
const int SERVO = 9;
const int STRIP = 6;
const int POT = 0;
const int CLK = 10;
const int DIO = 11;

// thresholds and limits
const int numPixels = 50;
const int leftPRThreshold = 350; // number falls below threshold on covering; above on release
const int rightPRThreshold = 550;
const int leftStart = 0;
const int leftEnd = 22;
const int rightStart = 23;
const int rightEnd = 42;
const int backStart = 43;
const int backEnd = 50;

// objects
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, STRIP, NEO_GRB + NEO_KHZ800);
Servo servo;
TM1637 tm1637(CLK, DIO);

// colors
uint32_t RED = strip.Color(255, 0, 0);
uint32_t GREEN = strip.Color(0, 255, 0);
uint32_t BLUE = strip.Color(0, 0, 255);
uint32_t WHITE = strip.Color(255, 255, 255);

int T = 0;

void setup() { 
  Serial.begin(9600);
  
  // LED strip
  strip.begin();
  strip.show();
  
  // servo
  servo.attach(SERVO);  // attaches the servo on pin 9 to the servo object 
  normalize();
  delay(150);
  servo.detach();
  
  // 7-segment LEDs
  tm1637.set();
  tm1637.init();
  Timer1.initialize(500000);//timing for 500ms
  Timer1.attachInterrupt(TimingISR);//declare the interrupt serve routine:TimingISR 
} 
 
void loop() { 

  set(WHITE, backStart, backEnd);
  int voltage = (analogRead(POT) * (5.0)); 
  int voltage2 = map(voltage, 0, 2000, -90, 90);
  Serial.println(String(analogRead(LEFTPR)) + ", " + String(analogRead(RIGHTPR)) + ", " + String(voltage2)); 

  if(Update == ON) {
    TimeUpdate();
    tm1637.display(TimeDisp);
  }

  int rgb2[] = {6, 0, 0};
  int rgb3[] = {6, 6, 0};
  if(analogRead(LEFTPR) > leftPRThreshold && analogRead(RIGHTPR) > rightPRThreshold) {
    normalize();
    delay(150);
     servo.detach();
    gentleFlash(leftStart, rightEnd, rgb2, 150);
  }
  
  else if(analogRead(LEFTPR) > leftPRThreshold) {
    //normalize();
    zeroExcept(backStart, backEnd);
    gentleFlash(leftStart, leftEnd, rgb2, 150);
  }
  
  else if(analogRead(RIGHTPR) > rightPRThreshold) {
    //normalize();
    zeroExcept(backStart, backEnd);
    gentleFlash(rightStart, rightEnd, rgb2, 150);
  }
    
  else if(analogRead(LEFTPR) < leftPRThreshold && analogRead(RIGHTPR) < rightPRThreshold) {
    servo.detach();
    set(BLUE, leftStart, rightEnd);
  }
  
  /*
  int angle = voltage;
  int leftpr = analogRead(LEFTPR) > leftPRThreshold;
  int rightpr = analogRead(RIGHTPR) > rightPRThreshold;
  
  if( T > 100 && T < 200) 
    Serial.println(String(angle) + "," + String(leftpr) + "," + String(rightpr) + "," + "d," + String(T));
  else {
    Serial.println(String(angle) + "," + String(leftpr) + "," + String(rightpr) + "," + "nd," + String(T));
    delay(1000);
  }
  T++;
  */
  
  Serial.println("ML," + String(voltage) + "," + String(analogRead(LEFTPR)) + "," + String(analogRead(RIGHTPR)));
  
  if(Serial.available() > 0) {
    char data = Serial.read();
    char str[2];
    str[0] = data;
    str[1] = '\0';
    if(String(str[0]).toInt() < 5) {
      zeroExcept(backStart, backEnd);
      gentleFlash(leftStart, rightEnd, rgb3, 150); 
    }
  }
}

/* NEOPIXEL LEDS */
void sequence(uint32_t c, uint8_t wait) {
  for(uint16_t i = rightStart; i < numPixels; i++) {
    strip.setPixelColor(i, c);
    Serial.println(i);
    strip.show();
    delay(wait);
  }
}

/* Flash with no post-dimming
Starting LED
Ending LED
R, G, or B (1, 2, 3) including scaling factor
Time to execute one dim-bright-dim cycle, in ms
*/
void abruptFlash(int startpoint, int endpoint, int rgb[], uint8_t period) {
  uint32_t color;
  for(int i = 0; i < 256; i++) {
    color = strip.Color(rgb[0]*i, rgb[1]*i, rgb[2]*i);
    for(uint16_t p = startpoint; p <= endpoint; p++)
      strip.setPixelColor(p, color);
    strip.show();
    delay(period/256);
  }
}

/* Flash with post-dimming
Starting LED
Ending LED
R, G, or B (1, 2, 3) including scaling factor
Time to execute one dim-bright-dim cycle, in ms
*/
void gentleFlash(int startpoint, int endpoint, int rgb[], uint8_t period) {
  uint32_t color;
  for(int i = 0; i < 256; i++) {
    color = strip.Color(rgb[0]*i, rgb[1]*i, rgb[2]*i);
    for(uint16_t p = startpoint; p <= endpoint; p++)
      strip.setPixelColor(p, color);
    strip.show();
    delay(period/256);
  } 
  for(int i = 255; i >= 0; i--) {
  color = strip.Color(rgb[0]*i, rgb[1]*i, rgb[2]*i);
    for(uint16_t p = startpoint; p <= endpoint; p++)
      strip.setPixelColor(p, color);
    strip.show();
    delay(period/256);
  }
}

void zero() {
  for(uint16_t p = 0; p < numPixels; p++)
    strip.setPixelColor(p, strip.Color(0, 0, 0));
  strip.show();
}

void zeroExcept(int startpoint, int endpoint) {
  for(uint16_t p = 0; p < numPixels; p++)
    if(p < startpoint || p > endpoint)
      strip.setPixelColor(p, strip.Color(0, 0, 0));
  strip.show();
}

void set(uint32_t c) {
  for(uint16_t p = 0; p < numPixels; p++)
    strip.setPixelColor(p, c);
  strip.show(); 
}

void set(uint32_t c, int startpoint, int endpoint) {
  for(uint16_t p = startpoint; p <= endpoint; p++)
    strip.setPixelColor(p, c);
  strip.show(); 
}

/* SERVO */
void normalize() {
  servo.attach(SERVO);
  servo.write(90);
}

/* 7-SEGMENT DISPLAY */
void TimingISR() {
  halfsecond ++;
  Update = ON;
  if(halfsecond == 2){
    second ++;
    if(second == 60)
    {
      minute ++;
      if(minute == 60)
      {
        hour ++;
        if(hour == 24)hour = 0;
        minute = 0;
      }
      second = 0;
    }
    halfsecond = 0;  
  }
 // Serial.println(second);
  ClockPoint = (~ClockPoint) & 0x01;
}

void TimeUpdate(void) {
  if(ClockPoint)tm1637.point(POINT_ON);
  else tm1637.point(POINT_OFF); 
  TimeDisp[0] = hour / 10;
  TimeDisp[1] = hour % 10;
  TimeDisp[2] = minute / 10;
  TimeDisp[3] = minute % 10;
  Update = OFF;
}
