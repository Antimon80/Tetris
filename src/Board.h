#ifndef BOARD_H
#define BOARD_H

#include "Display.h"
#include "Tetromino.h"

/**
 * @brief Forward declaration of the Tetromino class.
 *
 * This is necessary because the Board class references Tetromino objects,
 * but the full class definition is not required here.
 */
class Tetromino;

/**
 * @brief Constants defining the dimensions and offsets of the game board.
 */
#define BOARD_WIDTH 28
#define BOARD_HEIGHT 40
#define BOARD_OFFSET_X 3
#define BOARD_OFFSET_Y 21

/**
 * @brief The Board class represents the Tetris game board.
 *
 * This class manages the state of the game board, including the placement of
 * Tetromino pieces, collision detection, and clearing of full rows.
 */
class Board {
 private:
  /**
   * @brief A 2D array representing the state of the board.
   *
   * Each cell stores 3 bits to encode the type of Tetromino present, if any.
   * The board's width is stored compactly to save memory.
   */
  uint8_t field[BOARD_HEIGHT][(BOARD_WIDTH * 3 + 7) / 8];

 public:
  /**
   * @brief Constructor of the Board class.
   *
   * Initializes the game board by setting all cells to empty.
   */
  Board();

  /**
   * @brief Destructor of the Board class.
   *
   * Cleans up any resources used by the board. No specific actions are
   * performed as no dynamic memory is allocated.
   */
  ~Board();

  /**
   * @brief Retrieves the Tetromino type at a specific position on the board.
   *
   * Extracts the 3 bits representing the Tetromino type at the given
   * coordinates and returns the value as a TetrominoType.
   *
   * @param x The x-coordinate of the cell.
   * @param y The y-coordinate of the cell.
   * @return The TetrominoType at the specified position.
   */
  TetrominoType getFieldType(uint8_t x, uint8_t y) const;

  /**
   * @brief Computes the bit index for a given x-coordinate.
   *
   * Converts the x-coordinate of a cell to the starting bit index in the
   * compact board representation.
   *
   * @param x The x-coordinate of the cell.
   * @return The bit index for the cell.
   */
  uint16_t getBitIndex(uint8_t) const;

  /**
   * @brief Sets the Tetromino type at a specific position on the board.
   *
   * Updates the field array to store the specified Tetromino type at the
   * given coordinates.
   *
   * @param x The x-coordinate of the cell.
   * @param y The y-coordinate of the cell.
   * @param type The TetrominoType to set.
   */
  void setField(uint8_t x, uint8_t y, TetrominoType type);

  /**
   * @brief Renders the entire game board on the display.
   *
   * Iterates through the board and updates the display for each cell based
   * on the current state of the field.
   */
  void draw();

  /**
   * @brief Clears the entire game board.
   *
   * Resets all cells on the board to empty and updates the display to reflect
   * the cleared state.
   */
  void clear();

  /**
   * @brief Checks for collisions at a specified position and orientation.
   *
   * Determines if a Tetromino, when moved to the specified position and
   * rotation, would overlap with existing cells or exceed the board's
   * boundaries.
   *
   * @param tetromino The Tetromino to check.
   * @param targetX The target x-coordinate.
   * @param targetY The target y-coordinate.
   * @param targetRotation The target rotation index.
   * @return True if a collision occurs, otherwise false.
   */
  bool checkCollision(const Tetromino& tetromino, uint8_t targetX,
                      uint8_t targetY, uint8_t targetRotation);

  /**
   * @brief Places a Tetromino on the board.
   *
   * Updates the board state and visually draws the Tetromino on the display
   * at its current position and rotation.
   *
   * @param tetromino The Tetromino to place on the board.
   */
  void placeTetromino(const Tetromino& tetromino);

  /**
   * @brief Clears complete rows on the board.
   *
   * Checks for fully filled rows, removes them, and shifts the rows above
   * downward. Updates the display to reflect these changes.
   *
   * @return The number of rows cleared.
   */
  uint8_t clearFullLines();
};

#endif