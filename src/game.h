#ifndef GAME_H
#define GAME_H

#include "controller.h"
#include "map.h"
#include "player.h"
#include "renderer.h"

#include <memory>

enum class Direction { kUp, kDown, kLeft, kRight };

class Game {
public:
  Game();
  ~Game(){};

  bool initResources();
  void mainLoop();
  void freeResources();

private:
  std::unique_ptr<GameMap> gameMap;
  std::unique_ptr<GameRenderer> gameRenderer;
  std::shared_ptr<GameController<Movable>> gameController;
};

Game::Game() {
  gameMap = std::make_unique<GameMap>();
  gameRenderer = std::make_unique<GameRenderer>();
  gameController = std::make_shared<GameController<Movable>>();
}

#endif