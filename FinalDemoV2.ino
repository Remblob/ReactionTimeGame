#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>

float midi[127];
int A_four = 440;
int startSong[] = {60, 64, 67, 72};
int victorySong[] = {60, 62, 64, 65, 67, 69, 71, 72};
int lossSound[] = {65, 59};

const int switchPin = 7;
const int leftButtonPin = 4;

bool gameStart = false;
bool switchFlag = false;
bool switchVal = 0;
bool leftButtonFlag = false;

AsyncDelay timer;

int points = 0;

void setup() {

  CircuitPlayground.begin();
  attachInterrupt(digitalPinToInterrupt(switchPin), start, CHANGE);
  attachInterrupt(digitalPinToInterrupt(leftButtonPin), leftButtonPressed, FALLING);
  generateMIDI();

}

void loop() {

  
  if(switchFlag) {

    delay(5);
    gameStart = true;
    switchFlag = false;
    switchVal = digitalRead(switchPin);

  }

  if(gameStart == true && switchVal == 1) {

    startUp();
    lightTest();

  }

}

void startUp() {

  for(int i = 0; i < 10; i++) {
      CircuitPlayground.setPixelColor(i, 0, 255, 0);
  }
  
  for(int i = 0; i < 4; i++) {
    CircuitPlayground.playTone(midi[startSong[i]], 100);
  }

  CircuitPlayground.clearPixels();
  delay(500);

}

void generateMIDI() {

  for (int x = 0; x < 127; ++x) {
    midi[x] = (A_four / 32.0) * pow(2.0, ((x - 9.0) / 12.0));
    Serial.println(midi[x]);
  }

}

void startSound() {

  for(int i = 0; i < 4; i++) {
    CircuitPlayground.playTone(midi[startSong[i]], 100);
  }
  
}

void lightTest() {

  delay(random(2000, 4000));

  for(int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 0, 255, 0);
  }

  timer.start(333, AsyncDelay::MILLIS);
  delay(100);
  CircuitPlayground.clearPixels();

  if(timer.isExpired()) {
    gameOver();
  }

  if(leftButtonFlag) {
    points++;

    if(points == 10) {
      victory();
    }

  }

}

void victory() {

  for(int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 0, 255, 0);
  }

  for(int j = 0; j < 8; j++) {
    CircuitPlayground.playTone(midi[victorySong[j]], 500);
  }

  CircuitPlayground.clearPixels();
  gameStart == false;

}

void gameOver() {

  for(int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 0, 255, 0);
  }

  for(int j = 0; j < 2; j++) {
    CircuitPlayground.playTone(midi[lossSound[j]], 1000);
  }

  gameStart = false;
}

void start() {
  switchFlag = true;
}

void leftButtonPressed() {
  leftButtonFlag = true;
}
