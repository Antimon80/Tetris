#include "Tetromino.h"

#include "Board.h"
#include "Display.h"

/**
 * @brief Bit patterns defining Tetromino shapes in four rotations.
 *
 * Each entry in the array corresponds to a Tetromino type and its rotations.
 * The patterns are stored in program memory (PROGMEM) to save RAM.
 */
const uint64_t TETROMINOES[8][4] PROGMEM = {
    {0b0000000000000000000000000000000011111111111111110000000000000000ULL,
     0b0011000000110000001100000011000000110000001100000011000000110000ULL,
     0b0000000000000000000000000000000011111111111111110000000000000000ULL,
     0b0011000000110000001100000011000000110000001100000011000000110000ULL},  // I-TETRO
    {0b0000000000000000000000000000000000111100001111000011110000111100ULL,
     0b0000000000000000000000000000000000111100001111000011110000111100ULL,
     0b0000000000000000000000000000000000111100001111000011110000111100ULL,
     0b0000000000000000000000000000000000111100001111000011110000111100ULL},  // O-TETRO
    {0b0000000000000000000000000000000011111100111111000011000000110000ULL,
     0b0000000000000000001100000011000000111100001111000011000000110000ULL,
     0b0000000000000000001100000011000011111100111111000000000000000000ULL,
     0b0000000000000000001100000011000011110000111100000011000000110000ULL},  // T-TETRO
    {0b0000000000000000000000000000000011000000110000001111110011111100ULL,
     0b0000000000000000000011000000110000001100000011000011110000111100ULL,
     0b0000000000000000000000000000000011111100111111000000110000001100ULL,
     0b0000000000000000001111000011110000110000001100000011000000110000ULL},  // J-TETRO
    {0b0000000000000000000000000000000000000011000000110011111100111111ULL,
     0b0000000000000000001111000011110000001100000011000000110000001100ULL,
     0b0000000000000000000000000000000000111111001111110011000000110000ULL,
     0b0000000000000000001100000011000000110000001100000011110000111100ULL},  // L-TETRO
    {0b0000000000000000000000000000000000111100001111001111000011110000ULL,
     0b0000000000000000110000001100000011110000111100000011000000110000ULL,
     0b0000000000000000000000000000000000111100001111001111000011110000ULL,
     0b0000000000000000110000001100000011110000111100000011000000110000ULL},  // S-TETRO
    {0b0000000000000000000000000000000000111100001111000000111100001111ULL,
     0b0000000000000000000011000000110000111100001111000011000000110000ULL,
     0b0000000000000000000000000000000000111100001111000000111100001111ULL,
     0b0000000000000000000011000000110000111100001111000011000000110000ULL}  // Z-TETRO
};

/**
 * @brief Constructor for the Tetromino class.
 *
 * Initializes a Tetromino with its type, default rotation, position, and color.
 *
 * @param type The type of the Tetromino.
 */
Tetromino::Tetromino(TetrominoType type)
    : type(type), rotation(0), offsetX(0), offsetY(0), color(0) {}

/**
 * @brief Destructor for the Tetromino class.
 *
 * Cleans up resources used by the Tetromino object. Currently, no specific
 * actions are required since no dynamic memory is allocated.
 */
Tetromino::~Tetromino() {}

/**
 * @brief Retrieves the current rotation of the Tetromino.
 *
 * Returns the orientation of the Tetromino, which can range from 0 to 3.
 *
 * @return uint8_t The current rotation value (0, 1, 2, or 3).
 */
uint8_t Tetromino::getRotation() const { return rotation; }

/**
 * @brief Sets a new rotation for the Tetromino.
 *
 * Updates the rotation of the Tetromino to the specified value, ensuring it
 * wraps around within the valid range (0-3).
 *
 * @param newRotation The new rotation value to be set.
 */
void Tetromino::setRotation(uint8_t newRotation) { rotation = newRotation % 4; }

/**
 * @brief Retrieves the X-coordinate offset of the Tetromino.
 *
 * The offset determines the horizontal position of the Tetromino on the
 * display.
 *
 * @return uint8_t The current X-coordinate offset.
 */
uint8_t Tetromino::getOffsetX() const { return offsetX; }

/**
 * @brief Retrieves the Y-coordinate offset of the Tetromino.
 *
 * The offset determines the vertical position of the Tetromino on the board.
 *
 * @return uint8_t The current Y-coordinate offset.
 */
uint8_t Tetromino::getOffsetY() const { return offsetY; }

/**
 * @brief Sets the X and Y-coordinate offsets for the Tetromino.
 *
 * Updates the position of the Tetromino on the board by setting new horizontal
 * and vertical offsets.
 *
 * @param x The new X-coordinate offset.
 * @param y The new Y-coordinate offset.
 */
void Tetromino::setOffset(uint8_t x, uint8_t y) {
  offsetX = x;
  offsetY = y;
}

/**
 * @brief Retrieves the display color for a specific Tetromino type.
 *
 * Returns the RGB color associated with the given Tetromino type. These colors
 * are predefined and correspond to the visual appearance of the Tetrominos.
 *
 * @param type The Tetromino type for which the color is requested.
 * @return uint16_t The RGB color associated with the specified Tetromino type.
 */
uint16_t Tetromino::getColor(TetrominoType type) {
  switch (type) {
    case NO_TETRO:
      return Display::getColor(BLACK);
    case I_TETRO:
      return Display::getColor(CYAN);
    case O_TETRO:
      return Display::getColor(YELLOW);
    case T_TETRO:
      return Display::getColor(MAGENTA);
    case J_TETRO:
      return Display::getColor(BLUE);
    case L_TETRO:
      return Display::getColor(ORANGE);
    case S_TETRO:
      return Display::getColor(GREEN);
    case Z_TETRO:
      return Display::getColor(RED);
  }
}

/**
 * @brief Sets the color of the Tetromino.
 *
 * Updates the internal color attribute of the Tetromino to the specified value.
 * This color is used for rendering the Tetromino on the display.
 *
 * @param newColor The new color value to be assigned to the Tetromino.
 */
void Tetromino::setColor(uint16_t newColor) { color = newColor; }

/**
 * @brief Reads a 64-bit word from program memory (PROGMEM).
 *
 * Retrieves bit patterns for a Tetromino from flash memory, ensuring the data
 * is accessed correctly for drawing or collision checking.
 *
 * @param ptr Pointer to the 64-bit word in program memory.
 * @return uint64_t The retrieved bit pattern.
 */
uint64_t pgm_read_qword(const void* ptr) {
  uint64_t qword = 0;

  for (uint8_t row = 0; row < 8; row++) {
    uint8_t rowBits = pgm_read_byte((const uint8_t*)ptr + row);

    // Reverse the bits for proper alignment
    rowBits = (rowBits & 0xF0) >> 4 | (rowBits & 0x0F) << 4;
    rowBits = (rowBits & 0xCC) >> 2 | (rowBits & 0x33) << 2;
    rowBits = (rowBits & 0xAA) >> 1 | (rowBits & 0x55) << 1;

    qword |= ((uint64_t)rowBits << ((7 - row) * 8));
  }

  return qword;
}

/**
 * @brief Draws the Tetromino on the display.
 *
 * Iterates through the bit pattern of the Tetromino and renders each set bit
 * as a colored pixel on the display.
 *
 * @param offsetX The X-coordinate offset for drawing.
 * @param offsetY The Y-coordinate offset for drawing.
 */
void Tetromino::draw(uint8_t offsetX, uint8_t offsetY) {
  uint64_t shape = pgm_read_qword(&(TETROMINOES[type - 1][rotation]));
  for (uint8_t row = 0; row < 8; row++) {
    for (uint8_t col = 0; col < 8; col++) {
      if (shape & (1ULL << (row * 8 + col))) {
        matrix.drawPixel(offsetX + col, offsetY + row, color);
      }
    }
  }
}

/**
 * @brief Clears the Tetromino from the display.
 *
 * Removes the Tetromino's pixels by drawing over them with the background
 * color.
 *
 * @param offsetX The X-coordinate offset for clearing.
 * @param offsetY The Y-coordinate offset for clearing.
 */
void Tetromino::clear(uint8_t offsetX, uint8_t offsetY) {
  uint64_t shape = pgm_read_qword(&(TETROMINOES[type - 1][rotation]));
  for (uint8_t row = 0; row < 8; row++) {
    for (uint8_t col = 0; col < 8; col++) {
      if (shape & (1ULL << (row * 8 + col))) {
        matrix.drawPixel(offsetX + col, offsetY + row,
                         Display::getColor(BLACK));
      }
    }
  }
}

/**
 * @brief Moves the Tetromino to the left on the game board.
 *
 * This method checks for collisions on the board before moving the Tetromino
 * two units to the left. If a collision is detected, the movement is blocked.
 *
 * @param board Reference to the game board.
 * @return true if the movement was successful, false if it was blocked.
 */
bool Tetromino::moveLeft(Board& board) {
  if (board.checkCollision(*this, offsetX - 2, offsetY, rotation)) return false;
  offsetX -= 2;
  return true;
}

/**
 * @brief Moves the Tetromino to the right on the game board.
 *
 * This method checks for collisions on the board before moving the Tetromino
 * two units to the right. If a collision is detected, the movement is blocked.
 *
 * @param board Reference to the game board.
 * @return true if the movement was successful, false if it was blocked.
 */
bool Tetromino::moveRight(Board& board) {
  if (board.checkCollision(*this, offsetX + 2, offsetY, rotation)) return false;
  offsetX += 2;
  return true;
}
/**
 * @brief Moves the Tetromino down on the game board.
 *
 * This method checks for collisions on the board before moving the Tetromino
 * two units downward. If a collision is detected, the movement is blocked.
 *
 * @param board Reference to the game board.
 * @return true if the movement was successful, false if it was blocked.
 */
bool Tetromino::moveDown(Board& board) {
  if (board.checkCollision(*this, offsetX, offsetY + 2, rotation)) return false;
  offsetY += 2;
  return true;
}

/**
 * @brief Rotates the Tetromino to its next orientation.
 *
 * This method checks for collisions on the board before rotating the Tetromino
 * to its next orientation. If a collision is detected, the rotation is blocked.
 *
 * @param board Reference to the game board.
 * @return true if the rotation was successful, false if it was blocked.
 */
bool Tetromino::rotate(Board& board) {
  byte nextRotation = (rotation + 1) % 4;
  if (board.checkCollision(*this, offsetX, offsetY, nextRotation)) return false;
  rotation = nextRotation;
  return true;
}