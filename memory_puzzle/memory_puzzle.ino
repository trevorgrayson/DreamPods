
//set the pins where the buttons, LEDs and buzzer connect
const int BTN_COUNT = 8;
int button[] = {22, 23, 24, 25,     //red is button[0], yellow is button[1], green is button[2], blue is button[3]
                26, 27, 28, 29     
};  
int led[] =    {30, 31, 33, 32,     //red is led[0], yellow is led[1], green is led[2], blue is led[3]
                34, 35, 36, 37     // guesses 
};     
int tones[] = {262, 330, 392, 494}; //tones to play with each button (c, e, g, b)

int roundsToWin = 10;         //number of rounds the player has to play before they win the game (the array can only hold up to 16 rounds)
int buttonSequence[16];       //make an array of numbers that will be the sequence that the player needs to remember

int buzzerPin = 10;           //pin that the buzzer is connected to

int pressedButton = 4;        //a variable to remember which button is being pressed. 4 is the value if no button is being pressed.
int roundCounter = 1;         //keeps track of what round the player is on


long startTime = 0;           //timer variable for time limit on button press
long timeLimit = 2000;        //time limit to hit a button

boolean gameStarted = false;      //variable to tell the game whether or not to play the start sequence

void setup() {
  Serial.begin(9600);
  
  //set all of the button pins to input_pullup (use the built-in pull-up resistors)
  for (int x=0; x<BTN_COUNT; x++) 
    pinMode(button[x], INPUT_PULLUP);

  //set all of the LED pins to output
  for (int x=0; x<BTN_COUNT; x++)   
    pinMode(led[x], OUTPUT);

  pinMode(buzzerPin, OUTPUT);   //set the buzzer pin to output
}

void loop() {

  if (gameStarted == false) {   //if the game hasn't started yet
    startSequence();            //flash the start sequence
    roundCounter = 0;           //reset the round counter
    delay(1500);                //wait a second and a half
    gameStarted = true;         //set gameStarted to true so that this sequence doesn't start again
  }

  //each round, start by flashing out the sequence to be repeated
  for (int i = 0; i <= roundCounter; i++) { //go through the array up to the current round number
    flashLED(buttonSequence[i]); //turn on the LED for that array position and play the sound
    Serial.println(buttonSequence[i]);
    delay(200);                           //wait
    allLEDoff();                          //turn all of the LEDs off
    delay(200);
  }

  //then start going through the sequence one at a time and see if the user presses the correct button
  for (int i = 0; i <= roundCounter; i++) { //for each button to be pressed in the sequence

    startTime = millis();                 //record the start time

    while (gameStarted == true) { //loop until the player presses a button or the time limit is up (the time limit check is in an if statement)

      pressedButton = buttonCheck();      //every loop check to see which button is pressed

      if (pressedButton < 4) {            //if a button is pressed... (4 means that no button is pressed)

        flashLED(pressedButton);          //flash the LED for the button that was pressed

        if (pressedButton == buttonSequence[i]) { //if the button matches the button in the sequence
          delay(250);                   //leave the LED light on for a moment
          allLEDoff();                  //then turn off all of the lights and
          break;                        //end the while loop (this will go to the next number in the for loop)

        } else {                          //if the button doesn't match the button in the sequence
          loseSequence();               //play the lose sequence (the loose sequence stops the program)
          break;                        //when the program gets back from the lose sequence, break the while loop so that the game can start over
        }

      } else {                            //if no button is pressed
        allLEDoff();                      //turn all the LEDs off
      }

      //check to see if the time limit is up
      if (millis() - startTime > timeLimit) { //if the time limit is up
        loseSequence();                       //play the lose sequence
        break;                                //when the program gets back from the lose sequence, break the while loop so that the game can start over
      }
    }
  }

  if (gameStarted == true) {
    roundCounter = roundCounter + 1;      //increase the round number by 1

    if (roundCounter >= roundsToWin) {              //if the player has gotten to the 16th round
      winSequence();                      //play the winning song
    }

    delay(500);                           //wait for half a second between rounds
  }

}

//----------FUNCTIONS------------

//FLASH LED
void flashLED (int ledNumber) {
  Serial.println("flashing");
  digitalWrite(led[ledNumber],   HIGH);
  digitalWrite(led[ledNumber+4], HIGH);  // ditto for player2
  tone(buzzerPin, tones[ledNumber]);
}

//TURN ALL LEDS OFF
void allLEDoff () {
  //turn all the LEDs off
  for(int x=0; x<BTN_COUNT; x++)
    digitalWrite(led[x], LOW);
  //turn the buzzer off
  noTone(buzzerPin);
}

//CHECK WHICH BUTTON IS PRESSED
int buttonCheck() {
  //check if any buttons are being pressed
  if (digitalRead(button[0]) == HIGH) {
    Serial.println("RED ON");
    return 0;
  } else if (digitalRead(button[1]) == HIGH) {
    Serial.println("GREEN ON");
    return 1;
  } else if (digitalRead(button[2]) == HIGH) {
    Serial.println("BLUE ON");
    return 2;
  } else if (digitalRead(button[3]) == HIGH) {
    Serial.println("YELLOW ON");
    return 3;
  } else {
    return 4; //this will be the value for no button being pressed
  }
}

//START SEQUENCE
void startSequence() {
  Serial.println("Starting Sequence");
  randomSeed(analogRead(A0));   //make sure the random numbers are really random

  //populate the buttonSequence array with random numbers from 0 to 3
  for (int i = 0; i <= roundsToWin; i++) {
    buttonSequence[i] = round(random(0, 4));
  }

  //flash all of the LEDs 3 times when the game starts
  for (int i = 0; i < 3; i++) {

    tone(buzzerPin, tones[i], 200); //play one of the 4 tones

    //turn all of the leds on
    for(int x=0; x<BTN_COUNT; x++)
      digitalWrite(led[x], HIGH);

    delay(100);         //wait for a moment

    //turn all of the leds off
    for(int x=0; x<BTN_COUNT; x++)
      digitalWrite(led[x], LOW);
    
    delay(100);   //wait for a moment

  } //this will repeat 4 times
}

//WIN SEQUENCE
void winSequence() {
  Serial.println("Win!");
  //turn all the LEDs on
  for (int j = 0; j <= BTN_COUNT; j++) {
    digitalWrite(led[j], HIGH);
  }

  //play the 1Up noise
  tone(buzzerPin, 1318, 150);   //E6
  delay(175);
  tone(buzzerPin, 1567, 150);   //G6
  delay(175);
  tone(buzzerPin, 2637, 150);   //E7
  delay(175);
  tone(buzzerPin, 2093, 150);   //C7
  delay(175);
  tone(buzzerPin, 2349, 150);   //D7
  delay(175);
  tone(buzzerPin, 3135, 500);   //G7
  delay(500);

  //wait until a button is pressed
  do {
    pressedButton = buttonCheck();
  } while (pressedButton > 3);
  delay(100);

  gameStarted = false;   //reset the game so that the start sequence will play again.

}

//LOSE SEQUENCE
void loseSequence() {
  Serial.println("Lose!");
  //turn all the LEDs on
  for (int j = 0; j <= BTN_COUNT; j++) {
  //  digitalWrite(led[j], HIGH);
  
    
  }

  //play the 1Up noise
  tone(buzzerPin, 130, 250);   //E6
  delay(275);
  tone(buzzerPin, 73, 250);   //G6
  delay(275);
  tone(buzzerPin, 65, 150);   //E7
  delay(175);
  tone(buzzerPin, 98, 500);   //C7
  delay(500);

  //wait until a button is pressed
  do {
    pressedButton = buttonCheck();
  } while (pressedButton > 3);
  delay(200);

  gameStarted = false;   //reset the game so that the start sequence will play again.
}
