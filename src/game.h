#ifndef GAME_H
#define GAME_H

#include <algorithm>
#include <future>
#include <memory>
#include <thread>
#include <vector>

// forward declaration to avoid include cycles
class Sprite;
class Enemy;
class Player;
class GameMap;
class GameRenderer;
class GameInteraction;
class Movable;
template <class T> class GameController;

class Game {
public:
  Game();
  ~Game();

  bool initResources();
  void userInteraction(std::shared_ptr<Player> &);
  void actionPlayer(std::shared_ptr<Player>,
                    std::vector<std::shared_ptr<Sprite>>);
  void actionEnemy(std::shared_ptr<Enemy>,
                   std::vector<std::shared_ptr<Sprite>>);
  void mainLoop();

private:
  std::unique_ptr<GameMap> gameMap;
  std::unique_ptr<GameRenderer> gameRenderer;
  std::unique_ptr<GameInteraction> gameInteraction;
  std::shared_ptr<GameController<std::shared_ptr<Player>>> gameController;
  std::vector<std::thread> _threads;       // to improve latency
  std::vector<std::future<char>> _futures; // to improve throughput
  std::mutex _mtx;
  int _running = 1;
};

Game::Game() {
  gameMap = std::make_unique<GameMap>();
  gameRenderer = std::make_unique<GameRenderer>();
  gameInteraction = std::make_unique<GameInteraction>();
  gameController = std::make_shared<GameController<std::shared_ptr<Player>>>();
}

Game::~Game() {
  // set up thread barrier before this object is destroyed
  std::for_each(_threads.begin(), _threads.end(),
                [](std::thread &t) { t.join(); });
}

#endif