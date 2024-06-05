#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>

AsyncDelay delay_1s;
volatile bool switchFlag = false;
volatile bool buttonRightFlag = false;
volatile bool buttonLeftFlag = false;
bool switchState = false;
bool buttonRightState = false;
bool buttonLeftState = false;
const int buttonPinRight = 5;
const int buttonPinLeft = 4;
const int switchPin = 7;
unsigned long timePassed;
unsigned long startTime;
unsigned long timeElapsed;
int randDelay = random(10000);
volatile bool level1Activated = false;
volatile bool level2Activated = false;
volatile bool level3Activated = false;


void buttonLeftISR() {
  buttonLeftFlag = true;
}

void switchISR() {
  switchFlag = true;
}

void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  attachInterrupt(digitalPinToInterrupt(7), switchISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(4), buttonLeftISR, CHANGE);
}

void loop() {
  if (buttonLeftFlag) {
    delay(5);
    buttonLeftState = !buttonLeftState;
    buttonLeftFlag = false;
  }

  if (switchFlag == true) {
    startTime = 0;
    delay(randDelay);
    for (int i = 0; i < 10; i++) {
      CircuitPlayground.setPixelColor(i, 0, 255, 255);
    }
    delay(100);
    CircuitPlayground.clearPixels();
    level1Activated = true;
    startTime = millis() + 100;
    Serial.println("Timer Started");
    switchFlag = false;
  }

  if (buttonLeftState == true) {
    timeElapsed = millis() - startTime;
    Serial.print("Reaction Time: ");
    Serial.print(timeElapsed);
    Serial.println(" ms");
  }

  if (timeElapsed <= 500 && buttonLeftState == true && level1Activated == true && !level2Activated) {
    CircuitPlayground.playTone(100, 100);
    startTime = 0;
    delay(randDelay);
    for (int i = 0; i < 10; i++) {
      CircuitPlayground.setPixelColor(i, 255, 0, 0);
    }
    delay(100);
    CircuitPlayground.clearPixels();
    level2Activated = true;
    startTime = millis() + 100;
  }

  if (timeElapsed <= 250 && buttonLeftState == true && level2Activated == true && !level3Activated) {
    CircuitPlayground.playTone(100, 100);
    startTime = 0;
    delay(randDelay);
    for (int i = 0; i < 10; i++) {
      CircuitPlayground.setPixelColor(i, 0, 255, 0);
    }
    delay(100);
    CircuitPlayground.clearPixels();
    level3Activated = true;
    startTime = millis() + 100;
  }

  if (timeElapsed <= 150 && buttonLeftState == true && level3Activated == true) {
    CircuitPlayground.playTone(100, 100);
  }

  if (level1Activated && level2Activated && level3Activated) {
    level1Activated = false;
    level2Activated = false;
    level3Activated = false;
    startTime = 0;
  }
}