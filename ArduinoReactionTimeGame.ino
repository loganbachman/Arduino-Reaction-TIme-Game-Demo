#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>

volatile bool switchFlag = false;
volatile bool buttonLeftFlag = false;
bool switchState = false;
bool buttonLeftState = false;
const int buttonPinLeft = 4;
const int switchPin = 7;
unsigned long startTime; //variable that starts timer
unsigned long timeElapsed; //variable that counts time passed
int randDelay = random(10000); //sets random delay between 1-10 seconds
int score = 0;

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
    buttonLeftState = !buttonLeftState; //set button state to true
    buttonLeftFlag = false;
  }

  if (switchFlag == true) {
    startTime = 0;
    score = 0;
    delay(randDelay);
    for (int i = 0; i < 10; i++) {
      CircuitPlayground.setPixelColor(i, 0, 255, 255);
    }
    delay(100);
    CircuitPlayground.clearPixels();
    startTime = millis();
    switchFlag = false;
  }

  if (buttonLeftState == true) {
    timeElapsed = millis() - startTime; //converts start time to record time passed when button's pressed
    delay(5);
    Serial.print("Reaction Time: ");
    Serial.print(timeElapsed);
    Serial.println(" ms");
    Serial.print("Score: ");
    Serial.println(score);
  }

  if (timeElapsed <= 250 && buttonLeftState == true && score == 0) {
    score = 1; //change score as a new level has been reached
    CircuitPlayground.playTone(100, 200); //play sound to signify you reacted in time
    startTime = 0;
    delay(randDelay);
    for (int i = 0; i < 10; i++) {
      CircuitPlayground.setPixelColor(i, 255, 0, 0);
    }
    delay(100);
    CircuitPlayground.clearPixels();
    startTime = millis(); //set starttime
  } else if (timeElapsed <= 200 && buttonLeftState == true && score == 1) {
    CircuitPlayground.playTone(100, 200);
    score = 2;
    startTime = 0;
    delay(randDelay);
    for (int i = 0; i < 10; i++) {
      CircuitPlayground.setPixelColor(i, 0, 255, 0);
    }
    delay(100);
    CircuitPlayground.clearPixels();
    startTime = millis();
  } else if (timeElapsed <= 150 && buttonLeftState == true && score == 2) {
    CircuitPlayground.playTone(100, 200);
    score = 3;
    Serial.print("All Levels Completed!"); //end of game
  }
}
