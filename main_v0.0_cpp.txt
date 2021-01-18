#include <Arduino.h>

/* Start of Code */

#include "FastLED.h"
#include <Wire.h>

#define NUM_LEDS 8 // Number of LEDs per Stick
#define DATA_PIN_X 2 // Pin 2 connected to RGB X
#define DATA_PIN_Y 3 // Pin 3 connected to RGB Y

CRGB leds_X[NUM_LEDS];
CRGB leds_Y[NUM_LEDS];

/* The I2C address of the module */
#define HMC5803L_Address 0x1E

/* Register address for the X Y and Z data */
#define X 3
#define Y 7
#define Z 5

void setup() 
{
Serial.begin(9600); 
Wire.begin();
FastLED.addLeds<NEOPIXEL,DATA_PIN_X>(leds_X, NUM_LEDS);
FastLED.addLeds<NEOPIXEL,DATA_PIN_Y>(leds_Y, NUM_LEDS);

/* Initialise the module */ 
Init_HMC5803L();
}

void loop() 
{
/* Read each sensor axis data and output to the serial port */
Serial.print(HMC5803L_Read(X));
Serial.print(" ");
Serial.print(HMC5803L_Read(Y));
Serial.print(" ");
Serial.println(HMC5803L_Read(Z));

int xvalue = HMC5803L_Read(X);
int numLedsToLight = map(xvalue, -280, 600, 0, NUM_LEDS);
FastLED.clear();
for(int led = 0; led < numLedsToLight; led++) { 
if(led < 4)leds_X[led] = CRGB::Green;
if(led >=4 & led < 7)leds_X[led] = CRGB::Orange;
if(led >=7)leds_X[led] = CRGB::Red;
}

int yvalue = HMC5803L_Read(Y);
int numLedsToLight_1 = map(yvalue, -330, 330, 0, NUM_LEDS);
for(int led = 0; led < numLedsToLight_1; led++) { 
if(led < 4)leds_Y[led] = CRGB::Green;
if(led >=4 & led < 7)leds_Y[led] = CRGB::Orange;
if(led >=7)leds_Y[led] = CRGB::Red;
}
FastLED.setBrightness(50);
FastLED.show();

/* Wait a little before reading again */
delay(10);
}


/* This function will initialise the module and only needs to be run once
after the module is first powered up or reset */
void Init_HMC5803L(void)
{
/* Set the module to 8x averaging and 15Hz measurement rate */
Wire.beginTransmission(HMC5803L_Address);
Wire.write(0x00);
Wire.write(0x70);

/* Set a gain of 5 */
Wire.write(0x01);
Wire.write(0xA0);
Wire.endTransmission();
}


/* This function will read once from one of the 3 axis data registers
and return the 16 bit signed result. */
int HMC5803L_Read(byte Axis)
{
int Result;

/* Initiate a single measurement */
Wire.beginTransmission(HMC5803L_Address);
Wire.write(0x02);
Wire.write(0x01);
Wire.endTransmission();
delay(6);

/* Move modules the resiger pointer to one of the axis data registers */
Wire.beginTransmission(HMC5803L_Address);
Wire.write(Axis);
Wire.endTransmission();

/* Read the data from registers (there are two 8 bit registers for each axis) */ 
Wire.requestFrom(HMC5803L_Address, 2);
Result = Wire.read() << 8;
Result |= Wire.read();

return Result;
}

/* End of Code */

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}