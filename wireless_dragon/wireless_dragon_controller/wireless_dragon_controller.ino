/*
  This program should be uploaded to a four-button controller with an RF24 radio chip
  and used to control a two-wheeled device - such as a two-wheeled giant articulated dragon.

  Use four buttons - one for forwards, one for backwards, one for right, and one for left.
  The dragon expects to receive the readings of these buttons in that exact order.
  
  The code for the dragon is in wireless_dragon_body.ino.
*/
#include <SPI.h>
#include <RF24.h>

// Pin of the "go forwards" button
int F = 3;
// Pin of the "go backwards" button
int B = 4;
// Pin of the "go right" button
int R = 2;
// Pin of the "go left" button
int L = 5;

RF24 radio(9, 10);
const byte address[6] = "00001";

void setup() {
  pinMode(F, INPUT);
  pinMode(B, INPUT);
  pinMode(R, INPUT);
  pinMode(L, INPUT);
  Serial.begin(9600);

  // Set up the radio to listen for directions from our controller.
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
}

void loop() {
  // This message stores the button data that we will send to the dragon.
  // If the first array member is 1, it should drive forward; if the second is 1, it drives backward;
  // if the third is 1, it turns right; if the fourth is 1, it turns turn left.
  // If more than one member is 1, the lowest index that is 1 is prioritized.
  //             F, B, R, L
  bool msg[4] = { 0, 0, 0, 0 };

  // Push the readings from the buttons into the message.
  if (digitalRead(F)) {
    msg[0] = 1;
  }
  if (digitalRead(B)) {
    msg[1] = 1;
  }
  if (digitalRead(R)) {
    msg[2] = 1;
  }
  if (digitalRead(L)) {
    msg[3] = 1;
  }

  // RF24s are finicky; this debug code remains active so you can always
  // open the serial monitor and check what is and isn't being received.
  for (int i = 0; i < 4; ++i) {
    Serial.print(msg[i]);
  }
  Serial.println();

  // Send the message to the dragon body.
  radio.write(&msg, sizeof(msg));

  delay(10);
}
