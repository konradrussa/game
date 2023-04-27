#include "game.h"

bool Game::initResources() {
  bool mapLoaded = false;
  try {
    gameController->logging("Reading MAP");
    mapLoaded = gameMap->readMap();
    if (mapLoaded) {
      gameRenderer->setStates(&gameMap->getStates());
    }
  } catch (...) {
    mapLoaded = false;
    gameController->handleException(std::current_exception());
  }
  return mapLoaded;
}

void Game::mainLoop() {
  Player player;
  Enemy enemy;
  Sprite finish(State::kFinish);

  std::vector<std::vector<State>> &states = gameMap->getStates();

  for (auto it = states.begin(); it != states.end(); ++it) {
    int x = std::distance(states.begin(), it);
    std::vector<State> cols = *it;
    for (auto it2 = cols.begin(); it2 != cols.end(); ++it2) {
      int y = std::distance(cols.begin(), it2);
      SDL_Point point{x, y};
      if (State::kFinish == cols.at(y)) {
        finish.setCoordinates(std::move(point));
      }
      if (State::kPlayer == cols.at(y)) {
        player.setCoordinates(std::move(point));
      }
      if (State::kEnemy == cols.at(y)) {
        enemy.setCoordinates(std::move(point));
      }
    }
  }

  while (running) {
    running = gameInteraction->userInteraction(gameRenderer, player);
    gameRenderer->render();
  }
}

int main(int argCount, char **args) {
  Game game;

  if (!game.initResources()) {
    return EXIT_FAILURE;
  }

  game.mainLoop();

  return EXIT_SUCCESS;
}