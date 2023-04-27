#ifndef GAME_H
#define GAME_H

#include "controller.h"
#include "interaction.h"
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
  std::unique_ptr<GameInteraction> gameInteraction;
  std::shared_ptr<GameController<Movable>> gameController;
  std::vector<std::thread> _threads;         // to improve latency
  std::vector<std::promise<void>> _promises; // to improve throughput
  std::mutex _mtx;
  int running = 1;
};

Game::Game() {
  gameMap = std::make_unique<GameMap>();
  gameRenderer = std::make_unique<GameRenderer>();
  gameInteraction = std::make_unique<GameInteraction>();
  gameController = std::make_shared<GameController<Movable>>();
}

#endif