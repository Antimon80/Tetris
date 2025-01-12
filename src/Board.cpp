#include "Board.h"

#include "Display.h"
#include "Tetromino.h"

/**
 * @brief Constructor of the Board class.
 *
 * Initializes the playing field by setting all cells to empty (0).
 */
Board::Board() {
  for (uint8_t y = 0; y < BOARD_HEIGHT; y++) {
    for (uint8_t x = 0; x < (BOARD_WIDTH + 1) / 2; x++) {
      field[y][x] = 0;
    }
  }
}

/**
 * @brief Destructor of the Board class.
 *
 * Currently, it does not perform any actions since no dynamic memory is used.
 */
Board::~Board() {}

/**
 * @brief Computes the bit index for a given x-coordinate.
 *
 * Each horizontal cell in the field is represented by 3 bits. This method
 * calculates the starting bit index for the specified cell.
 *
 * @param x The x-coordinate of the cell.
 * @return The bit index for the cell.
 */
uint16_t Board::getBitIndex(uint8_t x) const { return x * 3; }

/**
 * @brief Retrieves the Tetromino type at a specific position on the board.
 *
 * This method extracts the 3 bits representing the Tetromino type stored at
 * the specified coordinates.
 *
 * @param x The x-coordinate of the cell.
 * @param y The y-coordinate of the cell.
 * @return The TetrominoType at the given coordinates, or NO_TETRO if empty.
 */
TetrominoType Board::getFieldType(uint8_t x, uint8_t y) const {
  uint16_t bitIndex = getBitIndex(x);
  uint16_t byteIndex = bitIndex / 8;
  uint8_t bitOffset = bitIndex % 8;

  uint8_t value = (field[y][byteIndex] >> bitOffset) & 0b111;

  if (bitOffset > 5) {
    value |= (field[y][byteIndex + 1] << (8 - bitOffset)) & 0b111;
  }

  return static_cast<TetrominoType>(value);
}

/**
 * @brief Sets the Tetromino type at a specific position on the board.
 *
 * Updates the 3 bits at the specified coordinates to store the given
 * TetrominoType.
 *
 * @param x The x-coordinate of the cell.
 * @param y The y-coordinate of the cell.
 * @param type The TetrominoType to be stored at the given position.
 */
void Board::setField(uint8_t x, uint8_t y, TetrominoType type) {
  uint16_t bitIndex = getBitIndex(x);
  uint16_t byteIndex = bitIndex / 8;
  uint8_t bitOffset = bitIndex % 8;

  field[y][byteIndex] &= ~(((1 << 3) - 1) << bitOffset);
  field[y][byteIndex] |= (static_cast<uint8_t>(type) & 0b111) << bitOffset;

  if (bitOffset > 5) {
    field[y][byteIndex + 1] &= ~((1 << (bitOffset - 5)) - 1);
    field[y][byteIndex + 1] |=
        (static_cast<uint8_t>(type) & 0b111) >> (8 - bitOffset);
  }
}

/**
 * @brief Places a Tetromino on the board and renders it on the display.
 *
 * Sets the cells of the board to the Tetromino type and visually draws the
 * Tetromino on the LED matrix display.
 *
 * @param tetromino The Tetromino object to place on the board.
 */
void Board::placeTetromino(const Tetromino& tetromino) {
  uint8_t boardX = tetromino.getOffsetX() - BOARD_OFFSET_X;
  uint8_t boardY = tetromino.getOffsetY() - BOARD_OFFSET_Y;

  uint64_t shape =
      pgm_read_qword(&(TETROMINOES[tetromino.type - 1][tetromino.rotation]));
  TetrominoType type = tetromino.type;

  for (uint8_t i = 0; i < 64; i++) {
    if (shape & (1ULL << i)) {
      uint8_t row = i / 8;
      uint8_t col = i % 8;

      uint8_t fieldX = boardX + col;
      uint8_t fieldY = boardY + row;

      if (fieldX < BOARD_WIDTH && fieldY < BOARD_HEIGHT) {
        setField(fieldX, fieldY, type);

        uint16_t color = Tetromino::getColor(type);
        matrix.drawPixel(fieldX + BOARD_OFFSET_X, fieldY + BOARD_OFFSET_Y,
                         color);
      }
    }
  }
}

/**
 * @brief Renders the entire board on the display.
 *
 * Iterates over each cell on the board and draws the corresponding color
 * on the LED matrix. Empty cells are drawn as black.
 */
void Board::draw() {
  for (uint8_t y = 0; y < BOARD_HEIGHT; y++) {
    for (uint8_t x = 0; x < BOARD_WIDTH; x++) {
      TetrominoType type = getFieldType(x, y);

      matrix.drawPixel(x + BOARD_OFFSET_X, y + BOARD_OFFSET_Y,
                       type != NO_TETRO ? Tetromino::getColor(type)
                                        : Display::getColor(BLACK));
    }
  }
}

/**
 * @brief Clears the board by resetting all cells to empty.
 *
 * Updates the internal field array and clears the display for all cells.
 */
void Board::clear() {
  for (uint8_t y = 0; y < BOARD_HEIGHT; y++) {
    for (uint8_t x = 0; x < (BOARD_WIDTH + 1) / 2; x++) {
      field[y][x] = 0;
    }
  }
}

/**
 * @brief Checks for collisions when moving or rotating a Tetromino.
 *
 * Evaluates if the Tetromino, when placed at the specified position and
 * rotation, would overlap with existing cells or exceed board boundaries.
 *
 * @param tetromino The Tetromino object to test.
 * @param targetX The target x-coordinate.
 * @param targetY The target y-coordinate.
 * @param targetRotation The target rotation index.
 * @return True if a collision occurs, otherwise false.
 */
bool Board::checkCollision(const Tetromino& tetromino, uint8_t targetX,
                           uint8_t targetY, uint8_t targetRotation) {
  uint8_t boardX = targetX - BOARD_OFFSET_X;
  uint8_t boardY = targetY - BOARD_OFFSET_Y;
  uint64_t shape =
      pgm_read_qword(&(TETROMINOES[tetromino.type - 1][targetRotation]));

  for (uint8_t i = 0; i < 64; i++) {
    if (shape & (1ULL << i)) {
      uint8_t row = i / 8;
      uint8_t col = i % 8;

      uint8_t fieldX = boardX + col;
      uint8_t fieldY = boardY + row;

      if (fieldX >= BOARD_WIDTH || fieldY >= BOARD_HEIGHT) {
        return true;
      }
      if (getFieldType(fieldX, fieldY) != NO_TETRO) {
        return true;
      }
    }
  }

  return false;
}

/**
 * @brief Clears complete lines and shifts rows above them downward.
 *
 * Iterates through the board to detect full rows, removes them, and shifts
 * rows above downward. Updates the display for all affected cells.
 *
 * @return The number of cleared rows.
 */
uint8_t Board::clearFullLines() {
  uint8_t clearedRows = 0;

  for (uint8_t y = 0; y < BOARD_HEIGHT; y += 2) {
    bool isFull = true;

    for (uint8_t x = 0; x < BOARD_WIDTH; x++) {
      if (getFieldType(x, y) == NO_TETRO ||
          getFieldType(x, y + 1) == NO_TETRO) {
        isFull = false;
        break;
      }
    }

    if (isFull) {
      clearedRows++;
      for (uint8_t row = y; row > 1; row -= 2) {
        for (uint8_t x = 0; x < BOARD_WIDTH; x++) {
          setField(x, row, getFieldType(x, row - 2));
          setField(x, row + 1, getFieldType(x, row - 1));

          matrix.drawPixel(x + BOARD_OFFSET_X, row + BOARD_OFFSET_Y,
                           getFieldType(x, row) != NO_TETRO
                               ? Tetromino::getColor(getFieldType(x, row))
                               : Display::getColor(BLACK));
          matrix.drawPixel(x + BOARD_OFFSET_X, row + 1 + BOARD_OFFSET_Y,
                           getFieldType(x, row + 1) != NO_TETRO
                               ? Tetromino::getColor(getFieldType(x, row + 1))
                               : Display::getColor(BLACK));
        }
      }

      for (uint8_t x = 0; x < BOARD_WIDTH; x++) {
        setField(x, 0, NO_TETRO);
        setField(x, 1, NO_TETRO);

        matrix.drawPixel(x + BOARD_OFFSET_X, 0 + BOARD_OFFSET_Y,
                         Display::getColor(BLACK));
        matrix.drawPixel(x + BOARD_OFFSET_X, 1 + BOARD_OFFSET_Y,
                         Display::getColor(BLACK));
      }
      y -= 2;
    }
  }
  return clearedRows;
}