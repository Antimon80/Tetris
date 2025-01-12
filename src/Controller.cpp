#include "Controller.h"

/**
 * @brief Constructor for the Controller class.
 *
 * Initializes the keypad object with the keymap, row pins, and column pins.
 * Sets the initial press start time to zero.
 */
Controller::Controller()
    : keypad(makeKeymap(keymap), (byte*)rowPins, (byte*)colPins, KEYPAD_ROWS,
             KEYPAD_COLS),
      pressStartTime(0) {}

/**
 * @brief Initializes the keypad and resets the press state.
 *
 * Configures the debounce time for the keypad to 10 milliseconds and waits
 * until no key is pressed before proceeding.
 */
void Controller::init() {
  keypad.setDebounceTime(10);  // Set debounce time for stable key detection
  pressStartTime = 0;

  // Wait until no key is pressed to ensure a clean initialization state
  while (keypad.getKey() != NO_KEY) {
  }
}

/**
 * @brief Checks if a specific key is currently being pressed.
 *
 * Reads the current key state from the keypad and compares it with the given
 * key.
 *
 * @param key The key to check.
 * @return True if the specified key is currently pressed, otherwise false.
 */
bool Controller::isKeyPressed(char key) {
  char keyPressed = keypad.getKey();  // Get the currently pressed key
  return keyPressed == key;  // Return true if it matches the specified key
}

/**
 * @brief Handles key presses and distinguishes between short and long presses.
 *
 * Detects the state of the currently pressed key and differentiates between
 * short presses (PRESSED state) and long presses (HOLD state). Long presses
 * are detected when the key is held down for at least 100 milliseconds.
 *
 * @return The character of the pressed key, or NO_KEY if no key is pressed.
 */
char Controller::handleKeyPress() {
  char key = keypad.getKey(); // Get the current key being pressed
  KeyState keyState = keypad.getState(); // Get the state of the key

  if (key != NO_KEY) { // If a key is pressed
    uint32_t currentTime = millis(); // Get the current time in milliseconds

    if (keyState == PRESSED) { // Handle a short press
      pressStartTime = currentTime; // Record the press start time
      return key; // Return the pressed key
    }
  }
  return NO_KEY; // Return NO_KEY if no valid key press is detected
}
