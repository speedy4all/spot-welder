#include <SevenSegmentExtended.h>
#include <SevenSegmentFun.h>
#include <Smooth.h>
#include <SevenSegmentTM1637.h>

int delayValue = 0;
int sensorValue = 0;
int oldValue = 0;

#define  SMOOTHED_SAMPLE_SIZE  50

#define LED_PIN 3
#define LED_POWER_PIN 4
#define BUTTON_PIN 7
#define RELAY_PIN 8

#define CLK 10
#define DIO 11

SevenSegmentExtended display(CLK, DIO);

int inputPin = A0;
int delayInputPin = A1;

// Smoothing average object
Smooth  average(SMOOTHED_SAMPLE_SIZE);
Smooth  delayAverage(SMOOTHED_SAMPLE_SIZE);

void setup()
{

  pinMode(inputPin, INPUT);
  pinMode(delayInputPin, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_POWER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  digitalWrite(LED_POWER_PIN, HIGH);
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  
  display.begin();
  display.setBacklight(100);
  display.print("INIT");
  delay(2000);
  display.clear();
}

void loop()
{

  average += analogRead(inputPin);
  delayAverage += analogRead(delayInputPin);
  

  sensorValue = (((int)average() / 10) * 10);
  delayValue = (((int)delayAverage() / 10) * 10);

  if(sensorValue == 0) {
    sensorValue = 10;
  }

  if(delayValue == 0) {
    delayValue = 10;
  }

  if(oldValue != sensorValue) {
    oldValue = sensorValue;
    display.clear();
    display.print(sensorValue);
  }

 
  if(isButtonPressed()) {
    activateWeld(delayValue);
    delay(500);
  }
  delay(1);
}

bool isButtonPressed() {
  bool pressed = digitalRead(BUTTON_PIN) == LOW;
  return pressed;
}

void activateWeld(int delayValue) {
  display.print("WELD"); 
  digitalWrite(LED_PIN, HIGH);
  delay(delayValue);
  
  digitalWrite(RELAY_PIN, HIGH);
  delay(sensorValue);
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  
  display.clear();
  display.print(sensorValue);
}

int smoothRead(){
  int i;
  int value = 0;
  int numReadings = 15;

  for (i = 0; i < numReadings; i++){
    // Read light sensor data.
    value = value + analogRead(inputPin);

    // 1ms pause adds more stability between reads.
    delay(1);
  }

  // Take an average of all the readings.
  value = value / numReadings;

  return value;
}