#include <Arduino.h>

#include "src/Controller.h"
#include "src/Game.h"

Game game;
Display display;
Controller controller;

extern HardwareSerial Serial;

void setup() {
  Serial.begin(9600);

  controller.init();
  display.initDisplay();

  while (true) {
    char key = controller.handleKeyPress();
    if (key == 'A') {
      break;
    }
    delay(100);
  }

  game.init();
}

void loop() {
  if (game.isGameOver()) {
    while (true) {
      char key = controller.handleKeyPress();
      if (key == 'C') {
        game.resetGame();
        game.init();
        break;
      }
    }
  } else {
    char key = controller.handleKeyPress();
    if (key != NO_KEY) {
      game.keyAction(key);
    }
    game.run();
  }
}