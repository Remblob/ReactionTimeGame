#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>
#include <iostream>

//All of the tones/melodies that play during the runtime of the game plus the midi function
float midi[127];
int A_four = 440;
int startSong[] = {60, 64, 67, 72};
int level2StartSong[] = {62, 66, 69, 74};
int level3StartSong[] = {64, 68, 71, 76};
int victorySong[] = {60, 62, 64, 65, 67, 69, 71, 72};
int lossSound[] = {65, 59};

//declaring input variables for the interrupts
const int switchPin = 7;
const int leftButtonPin = 4;
const int rightButtonPin = 5;

//all of the input logic - the flag variables trigger the interrupts, and the val variables actually affect the game
bool gameStart = false;
bool switchFlag = false;
bool switchVal = 0;
bool leftButtonFlag = false;
bool leftButtonVal = 0;
bool rightButtonFlag = 0;
bool rightButtonVal = 0;

//scoring system and timer system variables
int timer;
int points = 0;
int level = 1;

void setup() {

  //starts the circuit playground, sets up all the interrupts, and starts the midi function 
  CircuitPlayground.begin();
  attachInterrupt(digitalPinToInterrupt(switchPin), start, CHANGE);
  attachInterrupt(digitalPinToInterrupt(leftButtonPin), leftButtonPressed, RISING);
  attachInterrupt(digitalPinToInterrupt(rightButtonPin), rightButtonPressed, RISING);
  generateMIDI();

}

void loop() {

  //detects if the switch has been flipped 
  if(switchFlag) {

    //makes the game able to start and sets the switch value variable to the state of the switch
    delay(5);
    gameStart = true;
    switchFlag = false;
    switchVal = digitalRead(switchPin);

  }

  //checks if the game is able to start and if the switch is flipped the right way
  if(switchVal == 1 && gameStart == 1) {

    //indicates the game has started with a song according to the level the player is on, plays before every test
    startUp(level);

    //checks what level the player is on, and runs the relevant test
    if(level == 1) {
      lightTest();
    } else if(level == 2) {
      soundTest();
    } else if(level == 3) {
      
      //on level 3, the test you get is randomized
      int coinFlip = random(2);
      if(coinFlip == 0) {
        lightTest();
      } else {
        soundTest();
      }

    }

  }

}

//as previously stated, plays a melody before every test and is different according to player level
void startUp(int level) {

  //level 1: plays an arpeggiated C major chord while lighting up the neopixels in green
  if(level == 1) {

    for(int i = 0; i < 10; i++) {
      CircuitPlayground.setPixelColor(i, 0, 255, 0);
    }
  
    for(int i = 0; i < 4; i++) {
      CircuitPlayground.playTone(midi[startSong[i]], 100);
    }

  CircuitPlayground.clearPixels();
  delay(500);

  //level 2: plays an arpeggiated D major chord while lighting up the neopixels in yellow
  } else if(level == 2) {

    for(int i = 0; i < 10; i++) {
      CircuitPlayground.setPixelColor(i, 255, 255, 0);
    }
  
    for(int i = 0; i < 4; i++) {
      CircuitPlayground.playTone(midi[level2StartSong[i]], 100);
    }

  CircuitPlayground.clearPixels();
  delay(500);

  //level 3: plays an arpeggiated E major chord while lighting up the neopixels in orange
  } else if(level == 3) {

    for(int i = 0; i < 10; i++) {
      CircuitPlayground.setPixelColor(i, 255, 165, 0);
    }
  
    for(int i = 0; i < 4; i++) {
      CircuitPlayground.playTone(midi[level3StartSong[i]], 100);
    }

  CircuitPlayground.clearPixels();
  delay(500);

  }

}

//the first test of the game, tests visual reaction time
void lightTest() {

  //delays a random amount
  delay(random(1000, 4000));

    //sets both button flags to 0, prevents winning/losing loop
    leftButtonFlag = 0;
    rightButtonFlag = 0;

    //lights up all the neopixels in green
    for(int i = 0; i < 10; i++) {
      CircuitPlayground.setPixelColor(i, 0, 255, 0);
    }

    //timer that increases in value when buttons are not being pressed
    while(!leftButtonFlag && !rightButtonFlag) {

      delay(1);
      timer++;
      Serial.println(timer);

    }

    //checks for if the left button has been pressed
    if(leftButtonFlag) {
      leftButtonVal = 1;
    }

    //checks if left button was pressed in time 
    if(leftButtonVal && timer <= 333) {

      //resets timer and button value and increases player's points
      timer = 0;
      leftButtonVal = 0;
      points++;

      //flashes the neopixels white and plays a tone to indicate you pushed the right button in time
      for(int i = 0; i < 10; i++) {
        CircuitPlayground.setPixelColor(i, 255, 255, 255);
      }

      CircuitPlayground.playTone(midi[64], 200);
      CircuitPlayground.clearPixels();

      //checks player's point value to see if they can move on to the next level
      if(points == 5) {
        level = 2;

      //checks if player has enough points to win the whole game
      } else if(points == 20) {
        victory();
      }

    //checks if either the timer ran too long or if the wrong button was pressed to end the game
    } else if (timer > 333 || rightButtonFlag) {

      gameOver();

    }

}

//second test that tests auditory reaction time, pretty much the same code as the light test
void soundTest() {

  delay(random(1000, 4000));

  rightButtonFlag = 0;
  leftButtonFlag = 0;

  //major difference: plays tone instead of flashing neopixels
  CircuitPlayground.playTone(midi[71], 100);

  while(!rightButtonFlag && !leftButtonFlag) {

    delay(1);
    timer++;
    Serial.println(timer);

  }

  if(rightButtonFlag) {
    rightButtonVal = 1;
  }

  if(rightButtonVal && timer <= 250) {

    timer = 0;
    rightButtonVal = 0;
    points++;

    for(int i = 0; i < 10; i++) {
      CircuitPlayground.setPixelColor(i, 255, 255, 255);
    }

    CircuitPlayground.playTone(midi[64], 200);
    CircuitPlayground.clearPixels();

    if(points == 10) {
      level = 3;
    } else if(points == 20) {
      victory();
    }

  } else if (timer > 250 || leftButtonFlag) {

    gameOver();

  }
}

//function to make it easier to input tones into the code
void generateMIDI() {

  for (int x = 0; x < 127; ++x) {
    midi[x] = (A_four / 32.0) * pow(2.0, ((x - 9.0) / 12.0));
    Serial.println(midi[x]);
  }

}

//this function is called when you get enough points to win the whole game
void victory() {

  //sets neopixels to green color and plays "victory" C major scale
  for(int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 0, 255, 0);
  }

  for(int j = 0; j < 8; j++) {
    CircuitPlayground.playTone(midi[victorySong[j]], 150);
  }

  CircuitPlayground.clearPixels();

  //resets all of the game variables and turns off the game itself
  timer = 0;
  points = 0;
  level == 1;
  gameStart = false;

}

//pretty much the same as the victory function except it plays when you fail the game
void gameOver() {

  //turns all neopixels red and plays a "failure" tone
  for(int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 255, 0, 0);
  }

  for(int j = 0; j < 2; j++) {
    CircuitPlayground.playTone(midi[lossSound[j]], 500);
  }

  CircuitPlayground.clearPixels();

  //resets all of the game variables and turns off the game itself
  timer = 0;
  points = 0;
  level == 1;
  gameStart = false;

}

//all the ISR functions for the switch and button interrupts
void start() {
  switchFlag = true;
}

void leftButtonPressed() {
  leftButtonFlag = true;
}

void rightButtonPressed() {
  rightButtonFlag = true;
}
