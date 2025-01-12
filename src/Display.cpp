#include "Display.h"

#include <RGBmatrixPanel.h>
#include <avr/pgmspace.h>

#include "Fonts/FreeMonoBold9pt7b.h"
#include "Fonts/Picopixel.h"
#include "Tetromino.h"

/**
 * @brief Global instance of the RGB matrix panel for controlling the LED
 * display.
 */
RGBmatrixPanel matrix(A, B, C, D, E, CLK, LAT, OE, false, 64);

/**
 * @brief Color values for the display, stored in program memory.
 *
 * These values define the RGB color representation for each logical color in
 * the Colors enum.
 */
const uint16_t displayColors[NUM_COLORS] PROGMEM = {
    0xF81F,  // MAGENTA
    0x001F,  // BLUE
    0x07FF,  // CYAN
    0x07E0,  // GREEN
    0xFFE0,  // YELLOW
    0xFB40,  // ORANGE
    0xF800,  // RED
    0x0000,  // BLACK
    0x4208,  // GRAY
    0xFFFF   // WHITE
};

/**
 * @brief Retrieves the RGB value of a specified color.
 *
 * The color value is stored in program memory to save RAM space.
 *
 * @param color The color to retrieve.
 * @return The 16-bit RGB value of the specified color.
 */
uint16_t Display::getColor(Colors color) {
  return pgm_read_word(&(displayColors[color]));
}

const uint8_t COFFEE_CUP[26][22] PROGMEM = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1},
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1},
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1},
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1},
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0}};

const uint8_t MUSIC[20][9] PROGMEM = {
    {0, 0, 0, 0, 0, 0, 1, 1, 1}, {0, 0, 0, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 1, 1, 1, 1, 1, 1}, {0, 0, 0, 1, 1, 1, 0, 0, 1},
    {0, 0, 0, 1, 0, 0, 0, 0, 1}, {0, 0, 0, 1, 0, 0, 0, 0, 1},
    {0, 0, 0, 1, 0, 0, 1, 1, 1}, {0, 1, 1, 1, 0, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 0, 1, 1, 0}, {0, 1, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 1, 1, 1, 0},
    {0, 0, 0, 1, 1, 1, 1, 1, 0}, {0, 0, 0, 1, 1, 0, 0, 1, 0},
    {0, 0, 0, 1, 0, 0, 0, 1, 0}, {0, 0, 0, 1, 0, 0, 1, 1, 0},
    {0, 0, 1, 1, 0, 1, 1, 1, 0}, {0, 1, 1, 1, 0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}};

const char TITLE_LABEL[] PROGMEM = "TETRIS";
const char KEY_LABELS[][1] PROGMEM = {'A', 'B', '6', '4', '5', '2', '#', '*'};
const char CONTROL_LABELS[][7] PROGMEM = {"Start", "Pause",  "Left",
                                          "Right", "Rotate", "Down"};
const char GAME_LABELS[][6] PROGMEM = {"Level", "Score", "Next", "Lines"};
const char GAME_OVER[][19] PROGMEM = {"GAME OVER!", "Press C to Restart"};

const char* const STRINGS[] PROGMEM = {
    TITLE_LABEL,       KEY_LABELS[0],     KEY_LABELS[1],     KEY_LABELS[2],
    KEY_LABELS[3],     KEY_LABELS[4],     KEY_LABELS[5],     CONTROL_LABELS[0],
    CONTROL_LABELS[1], CONTROL_LABELS[2], CONTROL_LABELS[3], CONTROL_LABELS[4],
    CONTROL_LABELS[5], GAME_LABELS[0],    GAME_LABELS[1],    GAME_LABELS[2],
    GAME_LABELS[3],    GAME_OVER[0],      GAME_OVER[1],      KEY_LABELS[6],
    KEY_LABELS[7]};

const uint8_t POSITIONS[][2] PROGMEM = {
    {2, 16},   // TITLE
    {3, 26},   // A
    {34, 26},  // B
    {3, 34},   // 6
    {34, 34},  // 4
    {3, 42},   // 5
    {34, 42},  // 2
    {8, 26},   // START
    {39, 26},  // PAUSE
    {8, 34},   // LEFT
    {39, 34},  // RIGHT
    {8, 42},   // ROTATE
    {39, 42},  // DOWN
    {3, 1},    // Level-Label
    {34, 1},   // Score-Label
    {36, 20},  // Next-Label
    {34, 42},  // Lines-Label
    {11, 9},   // LEVEL
    {34, 9},   // SCORE
    {44, 29},  // NEXT
    {34, 50},  // LINES
    {2, 10},   // GAME OVER
    {2, 30},   // RESTART
    {21, 20},  // PAUSE
    {3, 50},   // #
    {34, 50},  // *
    {10, 47},  // VolUp
    {43, 36}   // VolDown
};

/**
 * @brief Initializes the RGB matrix display and renders the startup screen.
 *
 * This method performs the following steps:
 * 1. Initializes the display hardware.
 * 2. Clears the display by filling it with the BLACK color.
 * 3. Sets the font for the title and key labels.
 * 4. Displays the game title "TETRIS" at the top of the screen using a rainbow
 *    gradient of colors.
 * 5. Displays key labels and their positions on the screen.
 * 6. Displays control labels such as "Start", "Pause", "Left", etc.
 * 7. Displays musical indicators at specific positions for volume controls.
 *
 * @details
 * The method uses `pgm_read_byte` and `pgm_read_word` functions to access
 * strings, colors, and positions stored in program memory (PROGMEM), optimizing
 * RAM usage. The game title is drawn letter by letter with a gradient of
 * colors. Key and control labels are displayed in white and gray to provide
 * clear differentiation.
 */
void Display::initDisplay() {
  matrix.begin();
  matrix.fillScreen(Display::getColor(BLACK));

  matrix.setFont(&FreeMonoBold9pt7b);

  // Draw the game title "TETRIS"
  const char* title =
      reinterpret_cast<const char*> pgm_read_word((&STRINGS[0]));
  uint8_t x = pgm_read_byte(&POSITIONS[0][0]);
  uint8_t y = pgm_read_byte(&POSITIONS[0][1]);
  for (uint8_t i = 0; i < 6; i++) {
    uint8_t letterX = x + (i * 10);
    Colors colorIndex = static_cast<Colors>(MAGENTA + i);

    matrix.setCursor(letterX, y);
    matrix.setTextColor(Display::getColor(colorIndex));

    char letter = pgm_read_byte(&title[i]);
    matrix.print(letter);
  }

  matrix.setFont(&Picopixel);

  // Draw the key labels (e.g., A, B, 6, 4, etc.)
  for (uint8_t i = 1; i < 7; i++) {
    uint8_t xKey = pgm_read_byte(&POSITIONS[i][0]);
    uint8_t yKey = pgm_read_byte(&POSITIONS[i][1]);
    const char* key = reinterpret_cast<const char*>(pgm_read_word(&STRINGS[i]));
    char keyChar = pgm_read_byte(key);

    matrix.setCursor(xKey, yKey);
    matrix.setTextColor(Display::getColor(GRAY));
    matrix.print(keyChar);
  }

  // Draw the control labels (e.g., Start, Pause, Left, etc.)
  for (uint8_t i = 7; i < 13; i++) {
    uint8_t xLabel = pgm_read_byte(&POSITIONS[i][0]);
    uint8_t yLabel = pgm_read_byte(&POSITIONS[i][1]);
    const char* label =
        reinterpret_cast<const char*>(pgm_read_word(&STRINGS[i]));

    matrix.setCursor(xLabel, yLabel);
    matrix.setTextColor(Display::getColor(WHITE));
    while (char letter = pgm_read_byte(label++)) {
      matrix.print(letter);
    }
  }

  for (uint8_t i = 24; i < 26; i++) {
    uint8_t xKey = pgm_read_byte(&POSITIONS[i][0]);
    uint8_t yKey = pgm_read_byte(&POSITIONS[i][1]);
    const char* key =
        reinterpret_cast<const char*>(pgm_read_word(&STRINGS[i - 5]));
    char keyChar = pgm_read_byte(key);

    matrix.setCursor(xKey, yKey);
    matrix.setTextColor(Display::getColor(GRAY));
    matrix.print(keyChar);
  }

  uint8_t xUp = pgm_read_byte(&POSITIONS[26][0]);
  uint8_t yUp = pgm_read_byte(&POSITIONS[26][1]);

  matrix.setCursor(xUp, yUp);

  // Render the volume up icon as a blue note
  for (uint8_t row = 0; row < 10; row++) {
    for (uint8_t col = 0; col < 9; col++) {
      uint8_t pixel = pgm_read_byte(&(MUSIC[row][col]));
      if (pixel == 1) {
        matrix.drawPixel(xUp + col, yUp + row, Display::getColor(BLUE));
      }
    }
  }

  uint8_t xDown = pgm_read_byte(&POSITIONS[27][0]);
  uint8_t yDown = pgm_read_byte(&POSITIONS[27][1]);

  matrix.setCursor(xDown, yDown);

  // Render the volume down icon as a cyan note
  for (uint8_t row = 10; row < 20; row++) {
    for (uint8_t col = 0; col < 9; col++) {
      uint8_t pixel = pgm_read_byte(&(MUSIC[row][col]));
      if (pixel == 1) {
        matrix.drawPixel(xDown + col, yDown + row, Display::getColor(CYAN));
      }
    }
  }
}

/**
 * @brief Draws the static elements of the game interface.
 *
 * Includes the container frame and labels for level, score, next Tetromino,
 * and cleared lines.
 */
void drawStaticElements() {
  matrix.fillScreen(Display::getColor(BLACK));

  matrix.drawFastVLine(1, 19, 44, Display::getColor(GRAY));
  matrix.drawFastVLine(2, 19, 44, Display::getColor(GRAY));
  matrix.drawFastVLine(31, 19, 44, Display::getColor(GRAY));
  matrix.drawFastVLine(32, 19, 44, Display::getColor(GRAY));
  matrix.drawFastHLine(3, 19, 28, Display::getColor(GRAY));
  matrix.drawFastHLine(3, 20, 28, Display::getColor(GRAY));
  matrix.drawFastHLine(3, 61, 28, Display::getColor(GRAY));
  matrix.drawFastHLine(3, 62, 28, Display::getColor(GRAY));

  matrix.setFont(NULL);

  for (uint8_t i = 13; i < 17; i++) {
    uint8_t x = pgm_read_byte(&(POSITIONS[i][0]));
    uint8_t y = pgm_read_byte(&(POSITIONS[i][1]));
    const char* label =
        reinterpret_cast<const char*>(pgm_read_word((&STRINGS[i])));

    matrix.setCursor(x, y);
    matrix.setTextColor(Display::getColor(GRAY));
    matrix.print(reinterpret_cast<const __FlashStringHelper*>(label));
  }
}

/**
 * @brief Updates the displayed game level.
 *
 * Clears the previous level value and displays the new one.
 *
 * @param level The current game level.
 */
void updateLevelDisplay(uint8_t level) {
  uint8_t x = pgm_read_byte(&POSITIONS[17][0]);
  uint8_t y = pgm_read_byte(&POSITIONS[17][1]);

  matrix.fillRect(x, y, 11, 7, Display::getColor(BLACK));

  matrix.setFont(NULL);
  matrix.setCursor(x, y);
  matrix.setTextColor(Display::getColor(WHITE));
  if (level < 10) {
    matrix.print("0");
  }
  matrix.print(level);
}

/**
 * @brief Updates the displayed game score.
 *
 * Clears the previous score value and displays the new one.
 *
 * @param score The current game score.
 */
void updateScoreDisplay(uint16_t score) {
  uint8_t x = pgm_read_byte(&POSITIONS[18][0]);
  uint8_t y = pgm_read_byte(&POSITIONS[18][1]);

  matrix.fillRect(x, y, 30, 7, Display::getColor(BLACK));

  matrix.setFont(NULL);
  matrix.setCursor(x, y);
  matrix.setTextColor(Display::getColor(WHITE));

  if (score < 10000) matrix.print("0");
  if (score < 1000) matrix.print("0");
  if (score < 100) matrix.print("0");
  if (score < 10) matrix.print("0");

  matrix.print(score);
}

/**
 * @brief Updates the displayed number of lines cleared.
 *
 * Clears the previous value and displays the new one.
 *
 * @param lines The number of cleared lines.
 */
void updateLinesDisplay(uint16_t lines) {
  uint8_t x = pgm_read_byte(&POSITIONS[20][0]);
  uint8_t y = pgm_read_byte(&POSITIONS[20][1]);

  matrix.fillRect(x, y, 30, 7, Display::getColor(BLACK));

  matrix.setFont(NULL);
  matrix.setCursor(x, y);
  matrix.setTextColor(Display::getColor(WHITE));

  if (lines < 10000) matrix.print("0");
  if (lines < 1000) matrix.print("0");
  if (lines < 100) matrix.print("0");
  if (lines < 10) matrix.print("0");

  matrix.print(lines);
}

/**
 * @brief Displays the next Tetromino on the screen.
 *
 * Clears the previous preview and renders the new one.
 *
 * @param nextTetromino The Tetromino object to display as the next piece.
 */
void updateNextTetrominoDisplay(Tetromino& nextTetromino) {
  uint8_t x = pgm_read_byte(&POSITIONS[19][0]);
  uint8_t y = pgm_read_byte(&POSITIONS[19][1]);

  matrix.fillRect(x, y, 8, 8, Display::getColor(BLACK));

  nextTetromino.draw(x, y);
}

/**
 * @brief Displays the pause screen with a decorative icon.
 */
void pauseDisplay() {
  uint8_t x = pgm_read_byte(&POSITIONS[23][0]);
  uint8_t y = pgm_read_byte(&POSITIONS[23][1]);

  matrix.fillScreen(Display::getColor(BLACK));

  matrix.setCursor(x, y);

  uint8_t colorIndex = 0;

  // Draw the coffee cup icon with a color gradient
  for (uint8_t row = 0; row < 26; row++) {
    if (row % 2 == 0 && row > 0) {
      colorIndex = (colorIndex + 1) % 7;
    }

    uint16_t color = Display::getColor(static_cast<Colors>(colorIndex));

    for (uint8_t col = 0; col < 22; col++) {
      uint8_t pixel = pgm_read_byte(&(COFFEE_CUP[row][col]));

      if (pixel == 1) {
        matrix.drawPixel(x + col, y + row, color);
      }
    }
  }
}

/**
 * @brief Displays the "Game Over" screen and waits for a restart input.
 */
void gameOverDisplay() {
  uint8_t x1 = pgm_read_byte(&POSITIONS[21][0]);
  uint8_t y1 = pgm_read_byte(&POSITIONS[21][1]);
  uint8_t x2 = pgm_read_byte(&POSITIONS[22][0]);
  uint8_t y2 = pgm_read_byte(&POSITIONS[22][1]);

  matrix.fillScreen(Display::getColor(BLACK));

  // Display the "GAME OVER" text with a color gradient
  const char* gameOver =
      reinterpret_cast<const char*> pgm_read_word((&STRINGS[17]));
  for (uint8_t i = 0; i < 10; i++) {
    uint8_t letterX = x1 + (i * 6);
    Colors colorIndex = static_cast<Colors>(MAGENTA + (i % 7));

    matrix.setFont(NULL);
    matrix.setCursor(letterX, y1);
    matrix.setTextColor(Display::getColor(colorIndex));

    char letter = pgm_read_byte(&gameOver[i]);
    matrix.print(letter);
  }

  // Display the "Press C to Restart" text
  const char* reset =
      reinterpret_cast<const char*>(pgm_read_word((&STRINGS[18])));

  matrix.setCursor(x2, y2);
  matrix.setTextColor(Display::getColor(WHITE));
  matrix.print(reinterpret_cast<const __FlashStringHelper*>(reset));
}