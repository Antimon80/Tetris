#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Keypad.h>

/**
 * @brief Defines the number of rows and columns for the keypad.
 */
#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

/**
 * @brief Key mapping for the keypad.
 *
 * Maps each button on the 4x4 keypad to a character. These characters represent
 * the keys available to the player.
 */
const char keymap[KEYPAD_ROWS][KEYPAD_COLS] = {{'1', '2', '3', 'A'},
                                               {'7', '8', '9', 'C'},
                                               {'4', '5', '6', 'B'},
                                               {'*', '0', '#', 'D'}};

/**
 * @brief Pin configuration for the keypad rows and columns.
 *
 * Defines which microcontroller pins are connected to the rows and columns
 * of the keypad.
 */
const byte rowPins[KEYPAD_ROWS] = {37, 39, 41, 43};
const byte colPins[KEYPAD_COLS] = {45, 47, 49, 51};

/**
 * @brief The Controller class manages input handling through the keypad.
 *
 * This class initializes the keypad, processes key presses, and handles
 * debouncing and long-press detection.
 */
class Controller {
 private:
  Keypad keypad;            ///< Keypad object to manage input detection.
  uint32_t pressStartTime;  ///< Tracks the time when a key press starts.

 public:
  /**
   * @brief Constructor for the Controller class.
   *
   * Initializes the keypad object and sets the initial press start time to
   * zero.
   */
  Controller();

  /**
   * @brief Initializes the keypad and clears any existing key states.
   *
   * Configures the debounce time for the keypad and waits until no key is
   * pressed.
   */
  void init();

  /**
   * @brief Handles key presses and detects short or long presses.
   *
   * Checks the state of the keypad and determines if a key is being pressed
   * or held down for a long duration.
   *
   * @return The character of the pressed key, or NO_KEY if no key is pressed.
   */
  char handleKeyPress();

  /**
   * @brief Checks if a specific key is currently pressed.
   *
   * Compares the key currently detected by the keypad with the specified key.
   *
   * @param key The key to check.
   * @return True if the specified key is pressed, otherwise false.
   */
  bool isKeyPressed(char key);
};

#endif