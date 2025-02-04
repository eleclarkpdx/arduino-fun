/*
  This program drives a two-wheeled device - such as a giant articulated dragon on two wheels - 
  when it receives radio messages from a controller. 
  It must be controlled using a controller with four buttons - one for forwards, one for backwards,
  one for right, and one for left.
  This program expects to receive the readings of these buttons in that exact order.

  The code for the controller is in wireless_dragon_controller.ino.

  Eleanore Clark
*/

#include <SPI.h>
#include <RF24.h>

// The pins that connect to the motors.
// in1 and in2 go to the left motor; in3 and in4 go to the right motor.
const int in1 = 2;
const int in2 = 3;
const int in3 = 4;
const int in4 = 5;

RF24 radio(9, 10);
const byte address[6] = "00001";

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  Serial.begin(9600);

  // Set up the radio to listen for directions from our controller.
  radio.begin();
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  // This message stores the data that's sent to us from the controller.
  // If the first array member is 1, then we should drive forward; if the second is 1, we drive backward;
  // if the third is 1, we turn right; if the fourth is 1, we turn left.
  // If more than one member is 1, the lowest index that is 1 is prioritized.
  //             F, B, R, L
  bool msg[4] = {0, 0, 0, 0};
  
  if (radio.available()) {

    // Grab the message and put it in our array
    radio.read(&msg, sizeof(msg));
    // RF24s are finicky; this debug code remains active so you can always
    // open the serial monitor and check what is and isn't being received.
    for (int i = 0; i < 4; ++i) {
      Serial.print(msg[i]);
    }
    Serial.println();
    
    // If we were told to drive forward, drive forward.
    if (msg[0] == 1) {
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
    // If we were told to drive backward, drive backward.
    } else if (msg[1] == 1) {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    // If we were told to drive left, drive left.
    } else if (msg[2] == 1) {
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    // If we were told to drive right, drive right.
    } else if (msg[3] == 1) {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
    // If we were not told to drive, then don't drive.
    } else {
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
    }
  }

  delay(5);
}
