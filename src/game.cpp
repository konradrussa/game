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
    _running = 0;
    break;
  case '1':
    _running = 1;
    break;
  case 'l':
    player->action(Direction::kLeft, gameRenderer->getWorldSize());
    break;
  case 'r':
    player->action(Direction::kRight, gameRenderer->getWorldSize());
    break;
  case 'u':
    player->action(Direction::kUp, gameRenderer->getWorldSize());
    break;
  case 'd':
    player->action(Direction::kDown, gameRenderer->getWorldSize());
    break;
  }
}

// 12 threads SDL
//+1 main
//+2
// 15 threads
void Game::actionPlayer(std::shared_ptr<Player> player,
                        std::vector<std::shared_ptr<Sprite>> obstacles) {
  while (_running) {

    std::unique_lock<std::mutex> uLock(_mtx);
    gameController->send(std::move(player));
    uLock.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    // SDL_Delay(1000);
  }
}
void Game::actionEnemy(std::shared_ptr<Enemy> enemy,
                       std::vector<std::shared_ptr<Sprite>> obstacles) {
  while (_running) {

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
    int y = std::distance(states.begin(), it);
    std::vector<State> cols = *it;
    for (auto it2 = cols.begin(); it2 != cols.end(); ++it2) {
      int x = std::distance(cols.begin(), it2);
      SDL_Point point{x, y};
      if (State::kFinish == cols.at(x)) {
        finish->setCoordinates(std::move(point));
      }
      if (State::kPlayer == cols.at(x)) {
        player->setCoordinates(std::move(point));
      }
      if (State::kEnemy == cols.at(x)) {
        enemy->setCoordinates(std::move(point));
      }
      if (State::kObstacle == cols.at(x)) {
        std::shared_ptr<Sprite> obstacle(new Sprite(State::kObstacle));
        obstacle->setCoordinates(std::move(point));
        obstacles.emplace_back(std::move(obstacle));
      }
    }
  }

  gameRenderer->setObstaclesAndFinish(states.size(), obstacles, finish);

  _threads.emplace_back(
      std::thread(&Game::actionPlayer, this, player, obstacles));
  _threads.emplace_back(
      std::thread(&Game::actionEnemy, this, enemy, obstacles));

  while (_running) {
    std::promise<char> myPromise;
    std::future<char> myFuture = myPromise.get_future();

    std::unique_lock<std::mutex> uLock(_mtx);
    _futures.emplace_back(std::move(myFuture));
    gameInteraction->userInteraction(std::move(myPromise));
    userInteraction(player);
    uLock.unlock();

    gameRenderer->render(player, enemy);
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