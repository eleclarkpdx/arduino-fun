/*
  This is a completely-reworked version of Happy Train,
  UtahSAA's very short choose-your-own-adventure game.
  This project is meant to teach the use of functions to children 7-16.

  Use this project with a breadboard/PCB that has two buttons.
  They will both need pull-down resistors unless you change the input mode.
*/

// Declare the left and right buttons.
const int buttonL = 2;
const int buttonR = 3;

void setup() {
  Serial.begin(9600);
  pinMode(buttonL, INPUT);
  pinMode(buttonR, INPUT);
}

void loop() {
  // Offer the first choice
  bool gotOn = dialogue("You see a train!", "Get on.", "Don't.");
  // Deal with the first choice
  if (gotOn) {
    // Offer the second, and final, choice
    bool satInFront = dialogue("Will you sit in the front or back?","Front.","Back.");
    //Deal with the second, and final choice
    if (satInFront) {
      Serial.print("You watch train operators trade shifts for the rest of the journey.");
    }
    else {
      Serial.print("You sit in the back, and are lulled to sleep by the rocking of the train.");
    }
  }
  // Continue to deal with the first choice
  else {
    Serial.print("The train leaves you behind...");
  }

  dialogue("Press any button to play again!","Left button!","Right button!"); 
}

// Prints a prompt that lets the user know what kind of choice they're making,
// Prints the two options in the face of that choice (the left and right button options),
// Then take input from the user in the form of a button press.
// Advanced students should separate the button input into a separate function,
// Then call that function at an appropriate time (when?)
bool dialogue(char* prompt, char* optionL, char* optionR) {
  // Print the prompt and options
  Serial.println(prompt);
  delay(500);
  Serial.print("Left button:"); Serial.println(optionL);
  delay(500);
  Serial.print("Right button:"); Serial.println(optionR);
  Serial.println();

  //loop over the buttons until one is pressed
  while (true) { 
    if (digitalRead(buttonL) == 1) {
      return true;
    }
    if (digitalRead(buttonR) == 1) {
      return false;
    }
  }
}
