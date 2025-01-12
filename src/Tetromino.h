#ifndef TETROMINO_H
#define TETROMINO_H

#include <avr/pgmspace.h>

#include "Display.h"

class Board;

/**
 * @brief Declaration of Tetromino bit patterns as an external constant.
 *
 * These patterns define the shapes of each Tetromino in all four possible
 * rotations.
 */
extern const uint64_t TETROMINOES[8][4] PROGMEM;

/**
 * @brief Enumeration of Tetromino types.
 *
 * Each type represents a different Tetromino shape, as used in the Tetris game.
 */
enum TetrominoType {
  NO_TETRO = 0,  ///< Represents an empty space (no Tetromino).
  I_TETRO = 1,   ///< "I" Tetromino.
  O_TETRO = 2,   ///< "O" Tetromino.
  T_TETRO = 3,   ///< "T" Tetromino
  J_TETRO = 4,   ///< "J" Tetromino.
  L_TETRO = 5,   ///< "L" Tetromino.
  S_TETRO = 6,   ///< "S" Tetromino.
  Z_TETRO = 7    ///< "Z" Tetromino.
};

/**
 * @brief The Tetromino class represents a single Tetromino piece.
 *
 * This class includes methods for manipulating the Tetromino's position,
 * rotation, and appearance on the game board.
 */
class Tetromino {
 private:
  TetrominoType type;  ///< The type of the Tetromino.
  uint16_t color;      ///< The color associated with the Tetromino.
  uint8_t rotation;    ///< The current rotation of the Tetromino (0-3).
  uint8_t offsetX;     ///< The X-coordinate offset of the Tetromino.
  uint8_t offsetY;     ///< The Y-coordinate offset of the Tetromino.

 public:
  /**
   * @brief Constructor for the Tetromino class.
   *
   * @param type The type of the Tetromino.
   */
  Tetromino(TetrominoType type);

  /**
   * @brief Destructor for the Tetromino class.
   */
  ~Tetromino();

  /**
   * @brief Retrieves the current rotation of the Tetromino.
   *
   * The rotation represents the orientation of the Tetromino, ranging from 0
   * to 3.
   *
   * @return uint8_t The current rotation value (0, 1, 2, or 3).
   */
  uint8_t getRotation() const;

  /**
   * @brief Sets a new rotation for the Tetromino.
   *
   * Updates the rotation of the Tetromino to the specified value, ensuring it
   * remains within the valid range (0-3).
   *
   * @param newRotation The new rotation value to be set.
   */
  void setRotation(uint8_t newRotation);

  /**
   * @brief Retrieves the X-coordinate offset of the Tetromino.
   *
   * The offset determines the horizontal position of the Tetromino on the
   * display.
   *
   * @return uint8_t The current X-coordinate offset.
   */
  uint8_t getOffsetX() const;

  /**
   * @brief Retrieves the Y-coordinate offset of the Tetromino.
   *
   * The offset determines the vertical position of the Tetromino on the
   * display.
   *
   * @return uint8_t The current Y-coordinate offset.
   */
  uint8_t getOffsetY() const;

  /**
   * @brief Sets the X and Y-coordinate offsets for the Tetromino.
   *
   * Updates the position of the Tetromino on the display by setting new
   * horizontal and vertical offsets.
   *
   * @param x The new X-coordinate offset.
   * @param y The new Y-coordinate offset.
   */
  void setOffset(uint8_t x, uint8_t y);

  /**
   * @brief Retrieves the color for a specific Tetromino type.
   *
   * @param type The Tetromino type.
   * @return uint16_t The color associated with the specified Tetromino type.
   */
  static uint16_t getColor(TetrominoType type);

  /**
   * @brief Sets the color of the Tetromino.
   *
   * Updates the color attribute of the Tetromino to the specified value. This
   * color is used when rendering the Tetromino on the display.
   *
   * @param newColor The new color value to be assigned to the Tetromino.
   */
  void setColor(uint16_t newColor);

  /**
   * @brief Draws the Tetromino on the display at the specified offset.
   *
   * @param offsetX The X-coordinate offset.
   * @param offsetY The Y-coordinate offset.
   */
  void draw(uint8_t offsetX, uint8_t offsetY);

  /**
   * @brief Clears the Tetromino from the display at the specified offset.
   *
   * @param offsetX The X-coordinate offset.
   * @param offsetY The Y-coordinate offset.
   */
  void clear(uint8_t offsetX, uint8_t offsetY);

  /**
   * @brief Moves the Tetromino to the left.
   *
   * Checks for collisions before moving. If a collision occurs, the movement
   * is blocked.
   *
   * @param board Reference to the game board.
   * @return true if the movement was successful, false otherwise.
   */
  bool moveLeft(Board& board);

  /**
   * @brief Moves the Tetromino to the right.
   *
   * Checks for collisions before moving. If a collision occurs, the movement
   * is blocked.
   *
   * @param board Reference to the game board.
   * @return true if the movement was successful, false otherwise.
   */
  bool moveRight(Board& board);

  /**
   * @brief Moves the Tetromino downward.
   *
   * Checks for collisions before moving. If a collision occurs, the movement
   * is blocked.
   *
   * @param board Reference to the game board.
   * @return true if the movement was successful, false otherwise.
   */
  bool moveDown(Board& board);

  /**
   * @brief Rotates the Tetromino.
   *
   * Checks for collisions before rotating. If a collision occurs, the rotation
   * is blocked.
   *
   * @param board Reference to the game board.
   * @return true if the rotation was successful, false otherwise.
   */
  bool rotate(Board& board);

  friend class Board;  ///< Allows the Board class to access private members.
};

/**
 * @brief Reads a 64-bit word from program memory (PROGMEM).
 *
 * This function retrieves data stored in flash memory for use with Tetromino
 * shapes.
 *
 * @param ptr Pointer to the data in program memory.
 * @return uint64_t The 64-bit value read from memory.
 */
uint64_t pgm_read_qword(const void* ptr);

#endif