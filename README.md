# ReactionTimeGame
A game that tests your reaction time through the outputs of the Adafruit Circuit Playground Express.

The game I have chosen to develop is a reaction time based game. When you flip the switch, the game starts and a tone plays. The game is reaction time based, where either a tone will play or all of the neopixels will turn green, in which the player will have to press the corresponding button as soon as possible. There are 3 distinct levels: level 1 tests visual reaction time, level 2 tests auditory reaction time, and level 3 is a mix of both. In level 1, all the neopixels will turn green at once at a random time, and the player needs to press the right button as soon as possible. In level 2, a tone will play, and the player needs to press the left button as soon as possible. In level 3, either a tone will play or the neopixels will turn green, in which the player will have to press the corresponding button as soon as possible. The left button is for the light inputs, and the right button is for the sound inputs. If the player executes 10 correct inputs in a row, they pass the level and a victory tone will play. Once the third level is passed, a message saying that you’ve won will play and the game will reset. If the player fails to press the correct button within a certain time, they lose and a song plays to signify that it is game over. If a player gets a game over or wins the game, it resets and the switch must be flipped again in order to start a new game. 


Inputs and Outputs

Switch - Starts the game
Left Button - input for the light test
Right Button - input for the sound test
Speaker - outputs tones based on the game progression + one of the outputs the player reacts to
NeoPixels - one of the outputs the player reacts to

