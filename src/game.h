#ifndef GAME_H
#define GAME_H

#include "controller.h"
#include "map.h"
#include "player.h"
#include "renderer.h"

#include <memory>

class Game {
public:
  Game();
  ~Game(){};

  bool initResources();
  void mainLoop();

private:
  std::unique_ptr<GameMap> gameMap;
  std::unique_ptr<GameRenderer> gameRenderer;
  std::shared_ptr<GameController<Player>> gameController;
  std::vector<std::thread> _threads;
  SDL_Event event;
  int running = 1;
  bool fullScreen = false;
};

Game::Game() {
  gameMap = std::make_unique<GameMap>();
  gameRenderer = std::make_unique<GameRenderer>();
  gameController = std::make_shared<GameController<Player>>();
}

#endif