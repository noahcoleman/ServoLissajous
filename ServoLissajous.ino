/*
littleBits Arduino Module
 Original code by David Mellis http://www.arduino.cc/en/Tutorial/Graph
 created Jan 2009 by Trevor Shannon http://www.trevorshp.com/creations/etchasketch.htm
 modified Jan 2014 for littleBits by Kristin Salomon
 modified Mar 2014 for littleBits by Matt Richard
 modified May 2014 for littleBits by Richard Born
 modified Dec 2014 for my own nefarious ends by Noah Coleman
 */

#include <Servo.h>

// INPUTS
const int xFreq = A0; // input pin A0
const int yFreq = A1; // input pin A1
const int sizePot = A2; //pot for size
const int speedPot = A3; //pot for delay

// OUTPUTS
Servo servoX; // initialize x servo
Servo servoY; // initialize y servo

// VARIABLES
int x = 1500; // initial x position is at center of window
int y = 1500; // initial y position is at center of window
float t = 0; // parameter for Lissajous parametric equations
float phase = 0; // used to adjust phase for x-coordinate sin function
int oldX = 2; // holds the last value of the sin multiplier for the x coordinate
int oldY = 3; // holds the last value of the sin multiplier for the y coordinate
int xCoef = 0; // sin multiplier for x coordinate
int yCoef = 0; // sin multiplier for y coordinate
int sizeMult = 0;
int speedDelay = 0;

void setup() {
  servoX.attach(10); // attach x servo
  servoY.attach(11); // attach y servo
  //  Serial.begin(9600); // needed for serial debugging

  pinMode( xFreq, INPUT);
  pinMode( yFreq, INPUT);
}

void loop() {
  // Dividing the analog frequency by 250 gives 5 possible integers, dividing it by 113 gives 10
  xCoef = analogRead(xFreq)/250 + 1; // will give integer between 1 and 5 inclusive
  yCoef = analogRead(yFreq)/250 + 1; // will give integer between 1 and 5 inclusive
  sizeMult = analogRead(sizePot) + 1;
  speedDelay = analogRead(speedPot)/5 +1;
  // adjusting phase makes "prettier" Lissajous figures //NOAH NOTE: I don't know what this means or does
  if (xCoef % 2 == 1 || yCoef % 2 == 1) {
    phase = 1.5708; 
  } 
  else {
    phase = 0; 
  }

  // When the user changes any coefficient, we start the drawing over and save the new values
  if (xCoef != oldX || yCoef != oldY) {
    t = 0.0;
    oldX = xCoef;
    oldY = yCoef;
  }

  // Determine the x and y coordinates of the current position of the Lissajous being drawn
  // Drawing screen has center at (1500,1500)
  // x/y = MULTIPLIER(multiplier will +/- size of drawing, smaller=more pixely)*sin((# of waves)*3.14159*t + phase) + CENTER POINT
  x = sizeMult*sin(xCoef*3.14159*t + phase)+1500;
  y = sizeMult*sin(yCoef*3.14159*t)+1500;

  // Serial Debugging
//  Serial.print(x);
//  Serial.print(",");
//  Serial.println(y);

  // Write our Lissajous to the servos so they can draw it.
  servoX.writeMicroseconds(x);
  servoY.writeMicroseconds(y);

  t = t + .001*2*3.14159265; // increment the parameter t a little

  // Drawing speed delay--faster works better for lower frequency curves; consider tying it to the pots to automatically adjust
  delay(speedDelay*10);
}



