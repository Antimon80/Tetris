#ifndef DISPLAY_H
#define DISPLAY_H

#include <RGBmatrixPanel.h>
#include <avr/pgmspace.h>

/**
 * @brief Forward declaration of the Tetromino class.
 *
 * Required because Tetromino is referenced but not defined here.
 */
class Tetromino;

/**
 * @brief Definitions for the display pins connected to the Arduino.
 *
 * These constants represent the GPIO pins used for controlling the RGB LED
 * matrix.
 */
#define CLK 11
#define OE 9
#define LAT 10
#define A A0
#define B A1
#define C A2
#define D A3
#define E A4

/**
 * @brief Global instance of the RGB matrix panel.
 *
 * This instance provides access to display functionality and is externally
 * accessible.
 */
extern RGBmatrixPanel matrix;

/**
 * @brief Enum for defining display colors.
 *
 * Maps logical color names to 16-bit RGB color values.
 */
enum Colors {
  MAGENTA,
  BLUE,
  CYAN,
  GREEN,
  YELLOW,
  ORANGE,
  RED,
  BLACK,
  GRAY,
  WHITE,
  NUM_COLORS
};

/**
 * @brief Color values for the display, stored in program memory.
 *
 * This array defines the RGB values for each color in the Colors enum.
 */
extern const uint16_t displayColors[NUM_COLORS] PROGMEM;

/**
 * @brief The Display class provides methods for initializing and managing the
 * RGB LED matrix.
 *
 * This class contains static methods for initializing the display and
 * retrieving colors.
 */
class Display {
 public:
  /**
   * @brief Retrieves the RGB value of a specified color.
   *
   * @param color The color to retrieve.
   * @return The 16-bit RGB value of the specified color.
   */
  static uint16_t getColor(Colors color);

  /**
   * @brief Initializes the RGB matrix display.
   *
   * Sets up the display, clears it, and prepares it for rendering.
   */
  static void initDisplay();
};

/**
 * @brief Draws the static elements of the game interface on the display.
 */
void drawStaticElements();

/**
 * @brief Updates the level display on the screen.
 *
 * @param level The current game level to display.
 */
void updateLevelDisplay(uint8_t level);

/**
 * @brief Updates the score display on the screen.
 *
 * @param score The current score to display.
 */
void updateScoreDisplay(uint16_t score);

/**
 * @brief Updates the lines-cleared display on the screen.
 *
 * @param lines The number of lines cleared to display.
 */
void updateLinesDisplay(uint16_t lines);

/**
 * @brief Displays the next Tetromino on the screen.
 *
 * @param nextTetromino The next Tetromino object to display.
 */
void updateNextTetrominoDisplay(Tetromino& nextTetromino);

/**
 * @brief Displays the pause screen on the display.
 */
void pauseDisplay();

/**
 * @brief Displays the game over screen and waits for restart input.
 */
void gameOverDisplay();

#endif