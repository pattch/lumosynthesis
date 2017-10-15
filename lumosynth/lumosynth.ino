#include <Stepper.h>

const int sensorPin = 0;
const int ledPin = 6;

// We'll also set up some global variables for the light level:
int lightLevel;
int calibratedlightLevel; // used to store the scaled / calibrated lightLevel
int maxThreshold = 0;     // used for setting the "max" light level
int minThreshold = 1023;  // used for setting the "min" light level

const int srev = 200;     // change this to fit the number of steps per revolution
int motorSpeed = 1;
int motorPin1 = 11;
int motorPin2 = 10;
int motorPin3 = 9;
int motorPin4 = 8;
Stepper stl(srev, motorPin4,motorPin3,motorPin2,motorPin1);
int stepCount = 0;        // number of steps the motor has taken
int stepThreshold = 4096 * (180.0 / 360) / 8;

boolean opened = false;
boolean closed = true;
boolean opening = false;
boolean transitioning = false;

String msg = "";

int delayPeriod=100;
int delayTimer=100;

const int clockwisePin = 2;     // the number of the pushbutton pin
const int counterclockwisePin = 4;
int clockwisePinState = 0;
int counterclockwisePinState = 0;

void setup()
{
  pinMode(ledPin, OUTPUT);// Set up the LED pin to be an output.
  pinMode(motorPin1, OUTPUT);// Set up the LED pin to be an output.
  pinMode(motorPin2, OUTPUT);// Set up the LED pin to be an output.
  pinMode(motorPin3, OUTPUT);// Set up the LED pin to be an output.
  pinMode(motorPin4, OUTPUT);// Set up the LED pin to be an output.
  pinMode(clockwisePin, INPUT);
  Serial.begin(9600);
}

void loop()
{
  lightLevel = analogRead(sensorPin);   // reads the voltage on the sensorPin
  autoRange();                          // autoRanges the min / max values you see in your room.

  calibratedlightLevel = map(lightLevel, 0, 1023, 0, 255);
  printConstants();

  analogWrite(ledPin, calibratedlightLevel);    // set the led level based on the input lightLevel.

  handlePetals();
//  handleManual();
}

void openPetals()
{
  if(delayTimer != 0)
    return;
  else {
    closed = false;
    opened = false;
    transitioning = true;
    opening = true;
  }
}

void closePetals()
{
  if(delayTimer != 0)
    return;
  else {
    closed = false;
    opened = false;
    transitioning = true;
    opening = false;
  }
}

void handlePetals() {

  if(delayTimer > 0) {
    delayTimer--;
    msg = "Delaying.";
  } else {
    delayTimer = 0;
    if(transitioning) {
      handleTransition();
    } else if(closed) {
      handleClosed();
    } else if(opened) {
      handleOpened();
    } else {
      Serial.println("Invalid State! Neither Open, Closed, or in Transition!");
    }
  }
}

// Precondition: currently set to be transitioning
void handleTransition() {
  if(opening) {
    msg = "Opening";
    clockwise();
  } else {
    msg = "Closing";
    counterclockwise();
  }

  stepCount++;
  
  if(stepCount >= stepThreshold) {
    transitioning = false;
    // Arrived at our destination, set closed and opened variables
    closed = !opening;
    opened = opening;
    stepCount = 0;
    delayTimer = delayPeriod;
  }
}

void handleOpened() {
  msg = "Opened";
  if(lightLevel < 100)
    closePetals();
}

void handleClosed() {
  msg = "Closed";
  if(lightLevel > 150)
    openPetals();
}

void autoRange()
{
  if (lightLevel < minThreshold)  // minThreshold was initialized to 1023 -- so, if it's less, reset the threshold level.
    minThreshold = lightLevel;

  if (lightLevel > maxThreshold)  // maxThreshold was initialized to 0 -- so, if it's bigger, reset the threshold level.
    maxThreshold = lightLevel;

  // Once we have the highest and lowest values, we can stick them
  // directly into the map() function.
  // 
  // This function must run a few times to get a good range of bright and dark values in order to work.

  lightLevel = map(lightLevel, minThreshold, maxThreshold, 0, 255);
  lightLevel = constrain(lightLevel, 0, 255);
}

void printConstants()
{
  Serial.print("Light Level:\t");
  Serial.print(lightLevel);
  Serial.print("\tCalibrated Light Level");       // tab character
  Serial.print(calibratedlightLevel);   // println prints an CRLF at the end (creates a new line after)
  Serial.print("\tDelay Timer:\t");
  Serial.print(delayTimer);
  Serial.print("\tStep Count:\t");
  Serial.print(stepCount);
  Serial.println(msg);
}

//////////////////////////////////////////////////////////////////////////////
//set pins to ULN2003 high in sequence from 1 to 4
//delay "motorSpeed" between each pin setting (to determine speed)

void counterclockwise (){
 // 1
 digitalWrite(motorPin1, HIGH);
 digitalWrite(motorPin2, LOW);
 digitalWrite(motorPin3, LOW);
 digitalWrite(motorPin4, LOW);
 delay(motorSpeed);
 // 2
 digitalWrite(motorPin1, HIGH);
 digitalWrite(motorPin2, HIGH);
 digitalWrite(motorPin3, LOW);
 digitalWrite(motorPin4, LOW);
 delay (motorSpeed);
 // 3
 digitalWrite(motorPin1, LOW);
 digitalWrite(motorPin2, HIGH);
 digitalWrite(motorPin3, LOW);
 digitalWrite(motorPin4, LOW);
 delay(motorSpeed);
 // 4
 digitalWrite(motorPin1, LOW);
 digitalWrite(motorPin2, HIGH);
 digitalWrite(motorPin3, HIGH);
 digitalWrite(motorPin4, LOW);
 delay(motorSpeed);
 // 5
 digitalWrite(motorPin1, LOW);
 digitalWrite(motorPin2, LOW);
 digitalWrite(motorPin3, HIGH);
 digitalWrite(motorPin4, LOW);
 delay(motorSpeed);
 // 6
 digitalWrite(motorPin1, LOW);
 digitalWrite(motorPin2, LOW);
 digitalWrite(motorPin3, HIGH);
 digitalWrite(motorPin4, HIGH);
 delay (motorSpeed);
 // 7
 digitalWrite(motorPin1, LOW);
 digitalWrite(motorPin2, LOW);
 digitalWrite(motorPin3, LOW);
 digitalWrite(motorPin4, HIGH);
 delay(motorSpeed);
 // 8
 digitalWrite(motorPin1, HIGH);
 digitalWrite(motorPin2, LOW);
 digitalWrite(motorPin3, LOW);
 digitalWrite(motorPin4, HIGH);
 delay(motorSpeed);
}

//////////////////////////////////////////////////////////////////////////////
//set pins to ULN2003 high in sequence from 4 to 1
//delay "motorSpeed" between each pin setting (to determine speed)

void clockwise(){
 // 1
 digitalWrite(motorPin4, HIGH);
 digitalWrite(motorPin3, LOW);
 digitalWrite(motorPin2, LOW);
 digitalWrite(motorPin1, LOW);
 delay(motorSpeed);
 // 2
 digitalWrite(motorPin4, HIGH);
 digitalWrite(motorPin3, HIGH);
 digitalWrite(motorPin2, LOW);
 digitalWrite(motorPin1, LOW);
 delay (motorSpeed);
 // 3
 digitalWrite(motorPin4, LOW);
 digitalWrite(motorPin3, HIGH);
 digitalWrite(motorPin2, LOW);
 digitalWrite(motorPin1, LOW);
 delay(motorSpeed);
 // 4
 digitalWrite(motorPin4, LOW);
 digitalWrite(motorPin3, HIGH);
 digitalWrite(motorPin2, HIGH);
 digitalWrite(motorPin1, LOW);
 delay(motorSpeed);
 // 5
 digitalWrite(motorPin4, LOW);
 digitalWrite(motorPin3, LOW);
 digitalWrite(motorPin2, HIGH);
 digitalWrite(motorPin1, LOW);
 delay(motorSpeed);
 // 6
 digitalWrite(motorPin4, LOW);
 digitalWrite(motorPin3, LOW);
 digitalWrite(motorPin2, HIGH);
 digitalWrite(motorPin1, HIGH);
 delay (motorSpeed);
 // 7
 digitalWrite(motorPin4, LOW);
 digitalWrite(motorPin3, LOW);
 digitalWrite(motorPin2, LOW);
 digitalWrite(motorPin1, HIGH);
 delay(motorSpeed);
 // 8
 digitalWrite(motorPin4, HIGH);
 digitalWrite(motorPin3, LOW);
 digitalWrite(motorPin2, LOW);
 digitalWrite(motorPin1, HIGH);
 delay(motorSpeed);
}

void handleManual() {
  clockwisePinState = digitalRead(clockwisePin);
  counterclockwisePinState = digitalRead(counterclockwisePin);
  int manualSteps = 10;

  Serial.print(clockwisePinState);
  Serial.println(counterclockwisePinState);

  if(clockwisePinState == HIGH) {
    for(int i = 0; i < manualSteps; i++)
      clockwise();
  } else if(counterclockwisePinState == HIGH) {
    for(int i = 0; i < manualSteps; i++)
      counterclockwise();
  }
}

