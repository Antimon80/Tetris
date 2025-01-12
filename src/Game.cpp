#include "Game.h"

#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>

#include "Board.h"

// extern HardwareSerial Serial;
extern HardwareSerial
    Serial1;  ///< Serial interface for MP3 player communication.

// extern unsigned int __bss_end;
// extern unsigned int __heap_start;
// extern void* __brkval;

// int freeMemory() {
//   int free_memory;
//   if ((int)__brkval == 0) {
//     free_memory = ((int)&free_memory) - ((int)&__bss_end);
//   } else {
//     free_memory = ((int)&free_memory) - ((int)__brkval);
//   }
//   return free_memory;
// }

/**
 * @brief Constructor for the Game class.
 *
 * Initializes game variables, including score, level, board, and Tetromino
 * objects.
 */
Game::Game()
    : score(0),
      level(1),
      clearedRows(0),
      totalClearedRows(0),
      fallSpeed(1000),
      lastFallTime(0),
      isPaused(false),
      gameOver(false),
      board(),
      currentTetromino(nullptr),
      nextTetromino(nullptr) {}

/**
 * @brief Destructor for the Game class.
 *
 * Frees dynamically allocated memory for Tetromino objects.
 */
Game::~Game() {
  delete currentTetromino;
  currentTetromino = nullptr;
  delete nextTetromino;
  nextTetromino = nullptr;
}

/**
 * @brief Returns a reference to the game board object.
 *
 * @return Board& Reference to the game board object.
 */
Board& Game::getBoard() { return board; }

/**
 * @brief Returns a reference to the current Tetromino object.
 *
 * @return Tetromino& Reference to the current Tetromino object.
 */
Tetromino& Game::getCurrentTetromino() { return *currentTetromino; }

/**
 * @brief Checks whether the game is over.
 *
 * @return true if the game is over, otherwise false.
 */
bool Game::isGameOver() { return gameOver; }

/**
 * @brief Creates a new Tetromino with a random type and color.
 *
 * Allocates memory dynamically for a Tetromino and sets its type and color.
 *
 * @return Tetromino* Pointer to the newly created Tetromino.
 */
Tetromino* Game::createTetromino() {
  TetrominoType type;
  uint16_t color;

  // Randomly assign a Tetromino type and its corresponding color
  switch (random(18)) {
    case 0:
      type = I_TETRO;
      color = Display::getColor(CYAN);
      break;
    case 1:
    case 2:
      type = O_TETRO;
      color = Display::getColor(YELLOW);
      break;
    case 3:
    case 4:
    case 5:
      type = T_TETRO;
      color = Display::getColor(MAGENTA);
      break;
    case 6:
    case 7:
    case 8:
      type = J_TETRO;
      color = Display::getColor(BLUE);
      break;
    case 9:
    case 10:
    case 11:
      type = L_TETRO;
      color = Display::getColor(ORANGE);
      break;
    case 12:
    case 13:
    case 14:
      type = S_TETRO;
      color = Display::getColor(GREEN);
      break;
    case 15:
    case 16:
    case 17:
      type = Z_TETRO;
      color = Display::getColor(RED);
      break;
  }

  Tetromino* tetromino = new Tetromino(type);
  tetromino->setColor(color);
  return tetromino;
}

/**
 * @brief Initializes the game.
 *
 * This method sets up the display, the controller, and initializes all
 * relevant game variables. The game waits for player input before starting.
 */
void Game::init() {
  pinMode(BUZZER_PIN, OUTPUT);

  uint32_t startTime = millis();

  randomSeed(analogRead(A5));  // Seed the random generator

  // Initialize the MP3 player
  Serial1.begin(9600);
  if (!mp3Player.begin(Serial1)) {
    while (true);
  }
  mp3Player.volume(20);
  mp3Player.play(1);

  drawStaticElements();

  updateLevelDisplay(level);
  updateScoreDisplay(score);
  updateLinesDisplay(0);

  // Initialize Tetrominos
  currentTetromino = createTetromino();
  currentTetromino->setOffset(13, 17);
  nextTetromino = createTetromino();
  updateNextTetrominoDisplay(*nextTetromino);
}

/**
 * @brief Executes the main game loop.
 *
 * Handles Tetromino movement, collision checking, row clearing, and player
 * inputs.
 */
void Game::run() {
  uint32_t lastMemoryCheck = 0;

  if (mp3Player.available() && mp3Player.readType() == DFPlayerPlayFinished) {
    mp3Player.play(1);
  }

  uint32_t currentTime = millis();

  updateSounds();

  if (isPaused) {
    return;
  }

  // Handle Tetromino falling
  if (currentTime - lastFallTime >= fallSpeed) {
    currentTetromino->clear(currentTetromino->getOffsetX(),
                            currentTetromino->getOffsetY());
    if (!currentTetromino->moveDown(
            board)) {  // Check if Tetromino can move further
      board.placeTetromino(*currentTetromino);

      uint8_t rowsCleared = board.clearFullLines();
      totalClearedRows += rowsCleared;
      clearedRows += rowsCleared;
      updateScore(rowsCleared);
      updateLinesDisplay(totalClearedRows);
      updateLevelAndSpeed();

      delete currentTetromino;
      currentTetromino = nullptr;

      currentTetromino = nextTetromino;
      forceHeapReset();
      currentTetromino->setOffset(13, 17);

      // Check if the game is over
      if (board.checkCollision(*currentTetromino,
                               currentTetromino->getOffsetX(),
                               currentTetromino->getOffsetY(),
                               currentTetromino->getRotation())) {
        gameOverDisplay();
        mp3Player.stop();
        mp3Player.reset();
        gameOver = true;

        // Play game-over sound
        const uint16_t frequencies[] = {1200, 1000, 800};
        const uint16_t soundDurations[] = {300, 300, 800};

        for (uint8_t i = 0; i < 3; i++) {
          tone(BUZZER_PIN, frequencies[i]);
          delay(soundDurations[i]);
          noTone(BUZZER_PIN);
        }

        return;
      }

      // Prepare the next Tetromino
      nextTetromino = createTetromino();
      updateNextTetrominoDisplay(*nextTetromino);
    }
    currentTetromino->draw(currentTetromino->getOffsetX(),
                           currentTetromino->getOffsetY());
    lastFallTime = currentTime;
  }

  // if (currentTime - lastMemoryCheck >= 1000) {
  //   Serial.print("Freier Speicher: ");
  //   Serial.println(freeMemory());
  //   lastMemoryCheck = currentTime;
  // }
}

/**
 * @brief Handles player input and updates the Tetromino or game state.
 *
 * Processes key actions such as moving, rotating Tetrominos, pausing the game,
 * and adjusting the volume.
 *
 * @param key The character representing the pressed key.
 */
void Game::keyAction(char key) {
  Tetromino& currentTetromino = *this->currentTetromino;
  Board& board = this->board;

  currentTetromino.clear(currentTetromino.getOffsetX(),
                         currentTetromino.getOffsetY());

  switch (key) {
    case '6':
      currentTetromino.moveLeft(board);
      break;
    case '5':
      currentTetromino.rotate(board);
      break;
    case '4':
      currentTetromino.moveRight(board);
      break;
    case '2':
      currentTetromino.moveDown(board);
      break;
    case 'B':
      togglePause();
      break;
    case '#':
      mp3Player.volumeUp();
      break;
    case '*':
      mp3Player.volumeDown();
      break;
  }
}

/**
 * @brief Updates the score based on the number of cleared rows.
 *
 * Each number of cleared rows has a specific point multiplier based on the
 * current level.
 *
 * @param rowsCleared The number of rows cleared in the last move.
 */
void Game::updateScore(uint8_t rowsCleared) {
  if (rowsCleared > 0) {
    currentRowsCleared = rowsCleared;
    isRowClearSound = true;
    rowClearSoundStartTime = millis();
    rowClearSoundStep = 0;

    // Assign points based on rows cleared
    switch (rowsCleared) {
      case 1:
        score += 2 * level;
        break;
      case 2:
        score += 6 * level;
        break;
      case 3:
        score += 10 * level;
        break;
      case 4:
        score += 20 * level;
        break;
    }
    updateScoreDisplay(score);
  }
}

/**
 * @brief Increases the level and adjusts the Tetromino fall speed.
 *
 * Levels up the game after clearing a certain number of rows, making Tetrominos
 * fall faster.
 */
void Game::updateLevelAndSpeed() {
  if (clearedRows >= 10) {
    level++;
    clearedRows -= 10;
    fallSpeed = (fallSpeed > 100 ? fallSpeed - 100 : 100);
    updateLevelDisplay(level);

    if (isRowClearSound) {
      levelUpDelay = true;  // Introduce a delay for level-up sound
    }
  }
}

/**
 * @brief Plays the sound effect for clearing rows.
 *
 * This method produces a sequence of tones when rows are cleared. The sound
 * pattern depends on the number of rows cleared:
 * - Clearing 4 rows produces a unique alternating high and low tone pattern.
 * - Clearing fewer rows produces a simpler pattern.
 *
 * The method tracks time intervals to control the timing of each tone.
 */
void Game::rowClearSound() {
  if (millis() - rowClearSoundStartTime >= 200) {
    rowClearSoundStartTime = millis();

    if (currentRowsCleared == 4) {
      switch (rowClearSoundStep) {
        case 0:
          tone(BUZZER_PIN, 1200);
          break;
        case 1:
          tone(BUZZER_PIN, 800);
          break;
        case 2:
          tone(BUZZER_PIN, 1200);
          break;
        case 3:
          tone(BUZZER_PIN, 800);
          break;
        default:
          noTone(BUZZER_PIN);
          isRowClearSound = false;
      }
    } else {
      switch (rowClearSoundStep) {
        case 0:
          tone(BUZZER_PIN, 1200);
          break;
        case 1:
          tone(BUZZER_PIN, 800);
          break;
        default:
          noTone(BUZZER_PIN);
          isRowClearSound = false;
      }
    }
    rowClearSoundStep++;
  }
}

/**
 * @brief Plays the sound effect for leveling up.
 *
 * This method produces a sequence of ascending tones to signify that the
 * player has leveled up. Each tone is played for 100ms, and the method
 * progresses through the sequence based on elapsed time.
 *
 * After completing the sequence, the sound effect ends.
 */
void Game::levelUpSound() {
  if (millis() - levelUpSoundStartTime >= 100) {
    levelUpSoundStartTime = millis();

    switch (levelUpSoundStep) {
      case 0:
        tone(BUZZER_PIN, 1000);
        break;
      case 1:
        tone(BUZZER_PIN, 1200);
        break;
      case 2:
        tone(BUZZER_PIN, 1500);
        break;
      default:
        noTone(BUZZER_PIN);
        isLevelUpSound = false;
    }
    levelUpSoundStep++;
  }
}

/**
 * @brief Manages all game sound effects.
 *
 * This method determines which sound effect to play (row clear or level up)
 * based on the current game state. It ensures that only one sound effect is
 * active at a time and manages any required delays.
 *
 * - If `isRowClearSound` is true, the row clear sound is played.
 * - If `levelUpDelay` is true, it triggers the level-up sound effect.
 * - If `isLevelUpSound` is true, the level-up sound effect is played.
 */
void Game::updateSounds() {
  if (isRowClearSound) {
    rowClearSound();
    return;
  }

  if (levelUpDelay) {
    isLevelUpSound = true;
    levelUpDelay = false;
    levelUpSoundStartTime = millis();
    levelUpSoundStep = 0;
  }

  if (isLevelUpSound && !isRowClearSound) {
    levelUpSound();
    return;
  }
}

/**
 * @brief Toggles between paused and running states of the game.
 *
 * Pauses the game and updates the display. Resumes the game when unpaused.
 */
void Game::togglePause() {
  isPaused = !isPaused;

  if (isPaused) {
    mp3Player.pause();  // Pause the background music
    pauseDisplay();     // Show pause screen
  } else {
    mp3Player.start();  // Resume the background music
    drawStaticElements();
    updateLevelDisplay(level);
    updateScoreDisplay(score);
    updateLinesDisplay(totalClearedRows);
    updateNextTetrominoDisplay(*nextTetromino);

    // Redraw the board and the current Tetromino
    board.draw();

    currentTetromino->draw(currentTetromino->getOffsetX(),
                           currentTetromino->getOffsetY());
  }
}

/**
 * @brief Resets the game to start a new session.
 *
 * Clears the game board, resets all variables, and prepares for a new game.
 */
void Game::resetGame() {
  // Serial.print("Before reset, free memory: ");
  // Serial.println(freeMemory());

  // Delete existing Tetromino objects
  if (currentTetromino == nextTetromino) {
    delete currentTetromino;
    currentTetromino = nullptr;
    nextTetromino = nullptr;
  } else {
    if (currentTetromino) {
      delete currentTetromino;
      currentTetromino = nullptr;
    }
    if (nextTetromino) {
      delete nextTetromino;
      nextTetromino = nullptr;
    }
  }

  forceHeapReset();

  board.clear();

  // Reset game variables
  level = 1;
  score = 0;
  totalClearedRows = 0;
  clearedRows = 0;
  fallSpeed = 1000;
  lastFallTime = 0;
  isPaused = false;
  gameOver = false;

  // Serial.print("After reset, free memory: ");
  // Serial.println(freeMemory());
}

/**
 * @brief Resets the heap memory to avoid memory leaks.
 *
 * Allocates and frees a small block of memory to reset the heap state.
 */
void forceHeapReset() {
  void* testBlock = calloc(1, 1);
  if (testBlock) {
    free(testBlock);
  }
}
