#include "game.h"
#include "controller.h"
#include "interaction.h"
#include "map.h"
#include "player.h"
#include "renderer.h"

#include <chrono>
#include <random>

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

char Game::interaction() {
  char userKey = _futures.back().get();
  _futures.pop_back();

  switch (userKey) {
  case 'f':
    gameRenderer->fullscreen();
    break;
  case '0':
    _running = 0;
    break;
  case 'R':
    _finish = false;
    break;
  }
  return userKey;
}

void Game::userInteraction(std::shared_ptr<Player> &player) {
  char userKey = interaction();
  switch (userKey) {
  case 'l':
    player->action(Direction::kLeft);
    break;
  case 'r':
    player->action(Direction::kRight);
    break;
  case 'u':
    player->action(Direction::kUp);
    break;
  case 'd':
    player->action(Direction::kDown);
    break;
  }
}

// 12 threads SDL
//+1 main
//+2
// 15 threads
void Game::actionPlayer(std::shared_ptr<Player> player,
                        std::vector<std::shared_ptr<Sprite>> obstacles) {

  SDL_Point previousPlace{0, 0};
  while (_running) {
    std::unique_lock<std::mutex> _uLock(_mtx);
    SDL_Point currentPlace = player->getCoordinates();
    _uLock.unlock();
    if (previousPlace.x != currentPlace.x ||
        previousPlace.y != currentPlace.y) {
      _uLock.lock();
      gameController->send(std::move(currentPlace));
      _uLock.unlock();
      previousPlace.x = currentPlace.x;
      previousPlace.y = currentPlace.y;
    }

    std::this_thread::sleep_for(
        std::chrono::milliseconds(100)); // reduce CPU usage
    //    SDL_Delay(1000);
  }
  gameController->send(std::move(previousPlace));
}
//+1 thread from async
void Game::actionEnemy(std::shared_ptr<Enemy> enemy,
                       std::vector<std::shared_ptr<Sprite>> obstacles) {
  int low = 0, high = this->gameRenderer->getWorldSize() - 1;
  std::random_device rd;
  std::mt19937 eng(rd());
  std::uniform_int_distribution<int> distr(low, high);

  // int actionDistance = int(this->gameRenderer->getWorldSize()/4);

  while (_running) {

    std::future<SDL_Point> _playerFuture = std::async([this]() {
      return this->gameController->receive();
    }); // without std::launch::async, will be parallel or in same thread
    // chosen
    //  on runtime
    SDL_Point playerLocation = _playerFuture.get(); // or wait and get

    // OR
    // SDL_Point newPlayerPoint = gameController->receive();
    // waiting condition at receive

    // land at random position if distance was not close enough, x and y
    // further than 5 places, it's actionDistance 20/4
    SDL_Point point(playerLocation); // same as player
    point.x = distr(eng);            // override with random
    point.y = distr(eng);            // override with random
    std::unique_lock<std::mutex> _uLock(_mtx);
    enemy->setCoordinates(std::move(point));
    // if distance satisfy use regular move action
    enemy->action();
    _uLock.unlock();
  }
}

bool Game::checkFinish(std::shared_ptr<Player> &player,
                       std::shared_ptr<Enemy> &enemy,
                       std::shared_ptr<Sprite> &finish) {
  SDL_Point &playerLocation = player->getCoordinates();
  SDL_Point &enemyLocation = enemy->getCoordinates();
  SDL_Point &finishLocation = finish->getCoordinates();
  return ((playerLocation.x == finishLocation.x) &&
          (playerLocation.y == finishLocation.y)) ||
         ((enemyLocation.x == playerLocation.x) &&
          (enemyLocation.y == playerLocation.y));
}

void Game::mainLoop() {

  std::vector<std::vector<State>> &states = gameMap->getStates();

  std::vector<std::shared_ptr<Sprite>> obstacles;
  std::shared_ptr<Player> player(new Player(states.size()));
  std::shared_ptr<Enemy> enemy(new Enemy());
  std::shared_ptr<Sprite> finish(new Sprite(State::kFinish));

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
  player->setObstacles(obstacles);

  // run worker threads
  _threads.emplace_back(
      std::thread(&Game::actionPlayer, this, player, obstacles));
  _threads.emplace_back(
      std::thread(&Game::actionEnemy, this, enemy, obstacles));

  // evaluate frame
  std::chrono::milliseconds::rep targetFrameDuration =
      std::chrono::milliseconds(1000 / 60)
          .count(); // milliseconds per frame at 60 frames per second.

  std::chrono::steady_clock::time_point frameStart, frameEnd;
  std::chrono::milliseconds::rep duration;

  while (_running) {

    frameStart = std::chrono::steady_clock::now(); // 	 time in milliseconds

    std::promise<char> _myPromise;
    std::future<char> _myFuture = _myPromise.get_future();
    _futures.emplace_back(std::move(_myFuture));
    gameInteraction->userInteraction(
        std::move(_myPromise)); // should be async ...

    if (_finish) {
      interaction();
      gameRenderer->renderFinish();
    } else {
      std::unique_lock<std::mutex> _uLock(_mtx);
      userInteraction(player);
      _finish = checkFinish(player, enemy, finish);
      if (_finish) {
        SDL_Point moveToSaveLocation{0, 0};
        player->setCoordinates(std::move(moveToSaveLocation));
        _uLock.unlock();
      } else {
        _uLock.unlock();
        gameRenderer->render(player, enemy);
      }
    }

    // evaluate frame
    frameEnd = std::chrono::steady_clock::now(); // 	 time in milliseconds

    duration = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd -
                                                                     frameStart).count();

    // evaluate framerate
    if (duration < targetFrameDuration) {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(targetFrameDuration - duration));
    }
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