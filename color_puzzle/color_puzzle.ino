/***
 * color puzzle
 *
 * This is the first and simplest puzzle of the Dream Pod experience. 
 * Both players must stop the LED light strip cycle of 8 colors and the same color. 
 * Once both LED strips are synced, the puzzle is complete.
 *
 * TODO 
 * Thomas,
 * Here is my "best shot" trying to blind code this for you. 
 * 
 * I think we may actually be pretty close, so maybe try to plug this in
 * and give it a shot.
 *
 *
 * LedRGB library here = https://github.com/alexbertis/LibreriaLedRGB
 * MPR121 Library here = https://github.com/adafruit/Adafruit_MPR121
 *
 */

#include <Arduino.h>
#include <Wire.h>
#include <LedRGB.h> //Used for controlling the LED RGB strips
#include "Adafruit_MPR121.h" //Touch Capacitive shield library
#include <stdbool.h>

#define NUMBER_OF_COLOR 7  // this should be the total number of colors listed
#define MAX_STRING_SIZE 15 // this has to be longer than the longest color string.

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

char sequence[NUMBER_OF_COLOR][MAX_STRING_SIZE] = {"red", "green", "blue", "yellow", "orange", "violet", "turquoise"};

LedRGB p1Lights(22, 23, 24, CC);
LedRGB p2Lights(25, 26, 27, CC);

Adafruit_MPR121 cap = Adafruit_MPR121();

int p1seq = 0;
int p2seq = 3; // making this different to avoid them touching at the same time to beat the game.

int P1BUTTON = 1; // TODO set this to the pin that player1 is attached to on the cap sensor
int P2BUTTON = 2; // TODO set this to the pin that player1 is attached to on the cap sensor

void updatePlayer(int *player) {
   (*player)++;

   if (*player >= 7) {
       *player = 0;
   }
}


bool isP1touching() { // Is player 1 touching? set P1BUTTON to their pin.
    return cap.touched() & (1 << P1BUTTON);
}

bool isP2touching() { // Is player 2 touching? set P2BUTTON to their pin.
    return cap.touched() & (1 << P2BUTTON);
}

/**
 *
 * This method will be run on completion of the puzzle
 *
 */
void doTheHarlemShake() {
  for(int x=0; x<100; x++) {
    int color = x % 7;
    p1Lights.defaultColor(sequence[color]);
    p2Lights.defaultColor(sequence[color]);
    delay(200);
  }
}


void setup() {
  Serial.begin(9600);
  
  while (!Serial) { // needed to keep leonardo/micro from starting too fast!
    delay(10);
  }
  
  Serial.println("Adafruit MPR121 Capacitive Touch sensor test"); 
  
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1); // don't think you want this. This means you have to restart board?
  }
  Serial.println("MPR121 found!");
}

void loop() { updatePlayer(&p1seq); // update player colors
    updatePlayer(&p2seq);

    // set colors
    p1Lights.defaultColor(sequence[p1seq]); // change displayed lights
    p2Lights.defaultColor(sequence[p1seq]); // 

    printf("p1: %i p2: %i\n", p1seq, p2seq); // debug
    if (p1seq == p2seq && isP1touching() && isP2touching()) {
        doTheHarlemShake();
    }
    delay(1000);
}

