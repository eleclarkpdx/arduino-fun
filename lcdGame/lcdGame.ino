/*
  This program contains a simple game for the Arduino Uno.
  You control a smiley face, and you must navigate around enemy frowny faces to win.
  You lose if you ever share a tile with an enemy frowny face that is not asleep.
  You win by reaching the other end of the board.
  You need a 16x2 LCD in order to run this game, as well as four buttons.
  The pins associated with each button are stored in an enum in lcdGame.h, if you would like to edit them.

  Eleanore Clark
  2025.02.03
*/

#include <LiquidCrystal.h>
#include "lcdGame.h"
LiquidCrystal lcd(4, 6, 10, 11, 12, 13);

// Manually make a bunch of enemies, then store references to them in a list for easy access.
const int maxEnemies = 4;
player p(1, 1, playerSprite);
enemy e1(6, 1, enemySprite);
enemy e2(7, 0, enemySprite);
enemy e3(8, 1, enemySprite);
enemy e4(9, 0, enemySprite);
enemy* enemies[maxEnemies] = { &e1, &e2, &e3, &e4 };


void setup() {
  // General setup things
  randomSeed(analogRead(A0));
  lcd.begin(LCD_WIDTH, LCD_HEIGHT);
  initCustomChars();
  pinMode(ButtonPins::up, INPUT_PULLUP);
  pinMode(ButtonPins::right, INPUT_PULLUP);
  pinMode(ButtonPins::down, INPUT_PULLUP);
  pinMode(ButtonPins::left, INPUT_PULLUP);
  Serial.begin(9600);

  // Start screen
  lcd.setCursor(2, 0);
  lcd.print("GET TO THE");
  lcd.setCursor(1, 1);
  lcd.print("RIGHT-HAND SIDE");
  delay(2000);
}

void loop() {
  // This variable keeps track of whether the screen needs to refresh or not.
  // It's updated to true whenever anything moves,
  // and updated to false after the screen is cleared and re-drawn.
  bool needRefresh = false;

  // Move the player
  Direction dir = readButton();
  if (dir != noneDir) {
    delay(200);
    p.move(dir);
    needRefresh = true;
  }

  // Move and update the enemies
  for (int i = 0; i < maxEnemies; ++i) {
    enemies[i]->randomlyFlipAwakeness();
    ++enemies[i]->ticksSinceMoved;
    if (enemies[i]->randomlyMoveRandom()) {
      needRefresh = true;
    }
  }

  // Check if they player won/lost.
  // The player loses if they are hurt by an enemy,
  // which happens when the player shares a tile with an awake enemy.
  // The player wins if they make it to the other side of the screen.
  if (p.checkWin()) {
    exitWithMessage("YOU WIN!");
  }
  bool didLose = p.isHurtBy(enemies, maxEnemies);
  if (didLose) {
    exitWithMessage("YOU LOSE.");
  }

  // Refresh the screen if anything has moved
  if (needRefresh) {
    refresh();
  }
  delay(30);
}

// Clear the screen, then redraw the player and every enemy.
void refresh() {
  lcd.clear();
  lcd.setCursor(p.x, p.y);
  lcd.write(p.sprite);
  for (int i = 0; i < maxEnemies; ++i) {
    lcd.setCursor(enemies[i]->x, enemies[i]->y);
    lcd.write(enemies[i]->sprite);
  }
}

// Read from the four buttons on the breadboard/PCB.
// Return a direction corresponding with the button that's been pressed.
// If no button has been pressed, return noneDir.
Direction readButton() {
  bool up = !digitalRead(ButtonPins::up);
  bool right = !digitalRead(ButtonPins::right);
  bool down = !digitalRead(ButtonPins::left);
  bool left = !digitalRead(ButtonPins::down);

  if (up) return upDir;
  else if (right) return rightDir;
  else if (down) return downDir;
  else if (left) return leftDir;
  else return noneDir;
}

// Load all custom sprites into the LCD's memory.
void initCustomChars() {
  lcd.createChar(playerSprite, spriteList[playerSprite]);
  lcd.createChar(enemySprite, spriteList[enemySprite]);
  lcd.createChar(sleepingEnemySprite, spriteList[sleepingEnemySprite]);
}

// Stop the game, writing a message to the screen.
void exitWithMessage(char* msg) {
  refresh();
  lcd.setCursor(0, 0);
  lcd.print(msg);
  exit(0);
}
