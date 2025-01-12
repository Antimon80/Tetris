#ifndef GAME_H
#define GAME_H

#include <DFRobotDFPlayerMini.h>

#include "Board.h"

#define BUZZER_PIN 8  ///< Pin number for the buzzer used in the game sounds.

/**
 * @brief The Game class manages the main gameplay logic for a Tetris game.
 *
 * This class handles the game board, Tetromino management, scoring, levels,
 * player input, and game state transitions such as pause and game over.
 */
class Game {
 private:
  DFRobotDFPlayerMini
      mp3Player;                ///< MP3 player for background music and sounds.
  Board board;                  ///< The game board object.
  Tetromino* currentTetromino;  ///< Pointer to the currently active Tetromino.
  Tetromino* nextTetromino;     ///< Pointer to the next Tetromino.
  uint16_t score;               ///< Current game score.
  uint8_t level;                ///< Current game level.
  uint16_t clearedRows;       ///< Number of rows cleared in the current level.
  uint16_t totalClearedRows;  ///< Total number of rows cleared in the game.
  uint16_t fallSpeed;         ///< Speed at which Tetrominos fall (ms per step).
  uint32_t lastFallTime;      ///< Timestamp of the last fall step.

  bool isPaused;  ///< Indicates if the game is currently paused.
  bool gameOver;  ///< Indicates if the game is over.

  // Sound management variables
  uint8_t currentRowsCleared = 0;
  bool isRowClearSound = false;
  uint32_t rowClearSoundStartTime = 0;
  uint16_t rowClearSoundStep = 0;

  bool isLevelUpSound = false;
  bool levelUpDelay = false;
  uint32_t levelUpSoundStartTime = 0;
  uint16_t levelUpSoundStep = 0;

  /**
   * @brief Creates a new Tetromino with a random type and color.
   *
   * @return Tetromino* Pointer to the newly created Tetromino.
   */
  Tetromino* createTetromino();

  /**
   * @brief Updates the score based on the number of cleared rows.
   *
   * @param rowsCleared Number of rows cleared in the last move.
   */
  void updateScore(byte rowsCleared);

  /**
   * @brief Increases the level and adjusts the fall speed of Tetrominos.
   */
  void updateLevelAndSpeed();

  /**
   * @brief Handles the sound effects for clearing rows.
   */
  void rowClearSound();

  /**
   * @brief Handles the sound effects for leveling up.
   */
  void levelUpSound();

  /**
   * @brief Updates and manages all game sounds.
   */
  void updateSounds();

 public:
  /**
   * @brief Constructor for the Game class.
   *
   * Initializes all game variables and objects.
   */
  Game();

  /**
   * @brief Destructor for the Game class.
   *
   * Frees dynamically allocated objects to avoid memory leaks.
   */
  ~Game();

  /**
   * @brief Initializes the game.
   *
   * Sets up the game board, controls, and display, and waits for a start input.
   */
  void init();

  /**
   * @brief Executes the main game loop.
   *
   * Controls Tetromino movement, updates game states, and processes player
   * input.
   */
  void run();

  /**
   * @brief Processes player input based on the pressed key.
   *
   * @param key The character representing the player's input.
   */
  void keyAction(char key);

  /**
   * @brief Toggles the game's pause state.
   */
  void togglePause();

  /**
   * @brief Checks if the game is over.
   *
   * @return true if the game is over, otherwise false.
   */
  bool isGameOver();

  /**
   * @brief Resets the game to start a new session.
   *
   * Clears the board and resets all relevant game variables.
   */
  void resetGame();

  /**
   * @brief Returns a reference to the game board object.
   *
   * @return Board& Reference to the game board.
   */
  Board& getBoard();

  /**
   * @brief Returns a reference to the current Tetromino.
   *
   * @return Tetromino& Reference to the active Tetromino.
   */
  Tetromino& getCurrentTetromino();
};

/**
 * @brief Resets the heap memory to avoid memory leaks.
 *
 * Allocates and frees a small block of memory to reset the heap state.
 */
void forceHeapReset();

#endif
