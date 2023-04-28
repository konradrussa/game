#include "game.h"
#include "controller.h"
#include "interaction.h"
#include "map.h"
#include "player.h"
#include "renderer.h"

#include <chrono>

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

void Game::userInteraction(std::shared_ptr<Player> &player) {
  char userKey = _futures.back().get();
  _futures.pop_back();

  switch (userKey) {
  case 'f':
    gameRenderer->fullscreen();
    break;
  case '0':
    running = 0;
    break;
  case '1':
    running = 1;
    break;
  case 'l':
    player->left();
    break;
  case 'r':
    player->right();
    break;
  case 'u':
    player->up();
    break;
  case 'd':
    player->down();
    break;
  }
}

// 12 threads SDL
//+1 main
//+2
// 15 threads
void Game::actionPlayer(std::shared_ptr<Player> player,
                        std::vector<std::shared_ptr<Sprite>> obstacles) {
  while (running) {

    std::unique_lock<std::mutex> uLock(_mtx);
    gameController->send(std::move(player));
    uLock.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    // SDL_Delay(1000);
  }
}
void Game::actionEnemy(std::shared_ptr<Enemy> enemy,
                       std::vector<std::shared_ptr<Sprite>> obstacles) {
  while (running) {

    std::shared_ptr<Player> p = gameController->receive(); // waiting condition

    // enemy->action()

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    // SDL_Delay(1000);
  }
}

void Game::mainLoop() {

  std::vector<std::shared_ptr<Sprite>> obstacles;
  std::shared_ptr<Player> player(new Player());
  std::shared_ptr<Enemy> enemy(new Enemy());
  std::shared_ptr<Sprite> finish(new Sprite(State::kFinish));

  std::vector<std::vector<State>> &states = gameMap->getStates();

  for (auto it = states.begin(); it != states.end(); ++it) {
    int x = std::distance(states.begin(), it);
    std::vector<State> cols = *it;
    for (auto it2 = cols.begin(); it2 != cols.end(); ++it2) {
      int y = std::distance(cols.begin(), it2);
      SDL_Point point{x, y};
      if (State::kFinish == cols.at(y)) {
        finish->setCoordinates(std::move(point));
      }
      if (State::kPlayer == cols.at(y)) {
        player->setCoordinates(std::move(point));
      }
      if (State::kEnemy == cols.at(y)) {
        enemy->setCoordinates(std::move(point));
      }
      if (State::kObstacle == cols.at(y)) {
        std::shared_ptr<Sprite> obstacle(new Sprite(State::kObstacle));
        obstacle->setCoordinates(std::move(point));
        obstacles.emplace_back(std::move(obstacle));
      }
    }
  }

  _threads.emplace_back(
      std::thread(&Game::actionPlayer, this, player, obstacles));
  _threads.emplace_back(
      std::thread(&Game::actionEnemy, this, enemy, obstacles));

  while (running) {
    std::promise<char> myPromise;
    std::future<char> myFuture = myPromise.get_future();

    std::unique_lock<std::mutex> uLock(_mtx);
    _futures.emplace_back(std::move(myFuture));
    gameInteraction->userInteraction(std::move(myPromise));
    userInteraction(player);
    uLock.unlock();

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