/*
  This is a header file for my simple LCD game.
  It contains enum definitions, constant definitions, and struct definitions.
  There are no classes in the program.
*/

const int LCD_WIDTH = 16;
const int LCD_HEIGHT = 2;

// Directions. These are used to move entities around.
enum Direction {
  upDir,
  rightDir,
  downDir,
  leftDir,
  noneDir
};

// The pins attached to buttons on the breadboard/PCB.
enum ButtonPins {
  up = 2,
  down = 3,
  right = 5,
  left = 7,
};

// Use this to index the list of possible sprites immediately below.
enum Sprites {
  playerSprite = 0,
  enemySprite = 1,
  sleepingEnemySprite = 2,
};
// The custom sprites that represent various entities.
const byte spriteList[3][8] = {
  {
    B00000,
    B10001,
    B10001,
    B00000,
    B10001,
    B01110,
    B00100,
  },
  {
    B10001,
    B01010,
    B01010,
    B00000,
    B01110,
    B10001,
    B10001,
  },
  {
    B00000,
    B00000,
    B11011,
    B00000,
    B11111,
    B00000,
    B00000,
  },
};

// Entities have a sprite (what they look like)
// and a position in the form of two coordinates (where, on the LCD, the sprite is drawn.)
// They can move in any of four directions, but are constrained by the dimensions of the LCD.
struct entity {
  int x;
  int y;
  int sprite;

  // Attempt to move 1 tile in a given direction.
  // Returns true if the move is successful; false if not.
  bool move(Direction dir) {
    // Potential new coordinates to move to, depending on the direction
    int newY = y;
    int newX = x;
    bool coordChanged = false;

    switch (dir) {
      case upDir:
        --newY;
        break;
      case rightDir:
        ++newX;
        break;
      case downDir:
        ++newY;
        break;
      case leftDir:
        --newX;
        break;
      default:
        break;
    }

    // Clamp values so the entity doesn't go out-of-bounds
    if ((newX < LCD_WIDTH) and (newX > -1)) {
      x = newX;
      coordChanged = true;
    }
    if ((newY == 0) or (newY == 1)) {
      y = newY;
      coordChanged = true;
    }

    return coordChanged;
  }
};

// An enemy that is dangerous to players while awake, but harmless while asleep.
// Enemies move, sleep, and awake randomly. 
struct enemy : entity {
  bool isAsleep;
  int moveCooldown;
  int ticksSinceMoved;

  enemy(int newx, int newy, int newSprite) {
    x = newx;
    y = newy;
    sprite = newSprite;
    isAsleep = false;
    moveCooldown = 100;
    ticksSinceMoved = 0;
  }

  // Has a low chance to "flip" the enemy's awake status -
  // from awake to asleep, or from asleep to awake.
  // Returns true if the flip was performed.
  bool randomlyFlipAwakeness() {
    int dice = random(100);
    if (dice < 1) {
      isAsleep = !isAsleep;
      assignSprite();
      return true;
    } else {
      return false;
    }
  }

  // Re-assign the sprite to match the enemy's awake or asleep status.
  void assignSprite() {
    if (isAsleep) {
      sprite = sleepingEnemySprite;
    } else {
      sprite = enemySprite;
    }
  }

  // Random low chance to move 1 tile in a random direction, but only if
  // it's been a certain amount of time since the last move succeeded.
  // Returns false when the enemy doesn't move -
  // either because it is asleep,
  // because it failed the dice roll,
  // or because the cooldown isn't up.
  bool randomlyMoveRandom() {
    if (!isAsleep) {
      if (ticksSinceMoved > moveCooldown) {
        int dice = random(500);
        if (dice < 10) {
          Direction randDir = random(4);
          bool moved = move(randDir);
          while (!moved) {
            randDir = random(4);
            moved = move(randDir);
          }
          return true;
        }
      }
    }
    return false;
  }
};

// A player that is hurt by enemies and wins if they move to the goal at the end of the LCD.
struct player : entity {
  player(int newx, int newy, int newSprite) {
    x = newx;
    y = newy;
    sprite = newSprite;
  }

  // When given a list of enemies,
  // check to see if the player is intersecting with any that are awake.
  // If so, the player has been hurt by the enemy, and the function returns true.
  // If they player is not intersecting with any enemies, it returns false.
  bool isHurtBy(enemy** enemies, int enemyArrLength) {
    for (int i = 0; i < enemyArrLength; ++i) {
      if ((enemies[i]->x == x) and (enemies[i]->y == y) and (!enemies[i]->isAsleep)) {
        return true;
      }
    }
    return false;
  }

  // The player wins if they make it to the far side of the screen.
  // Return true if the player has won; false if not.
  bool checkWin() {
    if (x == LCD_WIDTH-1) return true;
    else return false;
  }
};