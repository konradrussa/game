#ifndef PLAYER_H
#define PLAYER_H

#include "astar.h"

#include <SDL2/SDL.h>
#include <memory>

class Sprite {
public:
  Sprite(){};
  ~Sprite(){};
  Sprite(const State &_state) : state(_state) {}
  Sprite(Sprite &);
  Sprite(Sprite &&);
  Sprite &operator=(Sprite &);
  Sprite &operator=(Sprite &&);

  void setCoordinates(SDL_Point &&point) { this->point = point; }
  SDL_Point &getCoordinates() { return point; }
  State &getState() { return state; }

protected:
  State state;
  SDL_Point point; // top, left, counter-clockwise in world-map coordinate 20x20
};

Sprite::Sprite(Sprite &other) {
  state = other.state;
  point = other.point;
}
Sprite::Sprite(Sprite &&other) {
  state = std::move(other.state);
  point = std::move(other.point);
}
Sprite &Sprite::operator=(Sprite &other) {
  if (this != &other) {
    state = other.state;
    point = other.point;
  }
  return *this;
}
Sprite &Sprite::operator=(Sprite &&other) {
  if (this != &other) {
    state = std::move(other.state);
    point = std::move(other.point);
  }
  return *this;
}

class Movable {
public:
  virtual void action() = 0; // pure virtual function
};

class Player : public Sprite, public Movable {
public:
  Player();
  ~Player();
  Player(const int worldSize) : Sprite(State::kPlayer), _worldSize(worldSize) {}
  Player(const State &_state, const int worldSize)
      : Sprite(_state), _worldSize(worldSize) {}
  Player(Player &);
  Player(Player &&);
  Player &operator=(Player &);
  Player &operator=(Player &&);

  void action() override;
  void action(const Direction direction);
  void setObstacles(std::vector<std::shared_ptr<Sprite>> &obstacles);
  bool checkNextActionObstacle(const Direction direction);

protected:
  int _worldSize;
  std::vector<std::shared_ptr<Sprite>> *obstacles;
};

class Enemy : public Sprite, public Movable {
public:
  Enemy();
  ~Enemy();
  Enemy(const State &_state) : Sprite(_state) {}
  Enemy(Enemy &);
  Enemy(Enemy &&);
  Enemy &operator=(Enemy &);
  Enemy &operator=(Enemy &&);

  void action() override;
};

Player::Player() : Sprite(State::kPlayer){};
Player::~Player(){};
Player::Player(Player &other) {
  state = other.state;
  point = other.point;
  _worldSize = other._worldSize;
  *obstacles = *other.obstacles;
}
Player::Player(Player &&other) {
  state = std::move(other.state);
  point = std::move(other.point);
  _worldSize = std::move(other._worldSize);
  other._worldSize = 0;
  obstacles = other.obstacles;
  other.obstacles = nullptr;
}
Player &Player::operator=(Player &other) {
  if (this != &other) {
    state = other.state;
    point = other.point;
    if (obstacles) {
      delete obstacles;
    }
    *obstacles = *other.obstacles;
    _worldSize = other._worldSize;
  }
  return *this;
}
Player &Player::operator=(Player &&other) {
  if (this != &other) {
    state = std::move(other.state);
    point = std::move(other.point);
    if (obstacles) {
      delete obstacles;
    }
    _worldSize = other._worldSize;
    other._worldSize = 0;
    obstacles = other.obstacles;
    other.obstacles = nullptr;
  }
  return *this;
}

void Player::setObstacles(std::vector<std::shared_ptr<Sprite>> &obstacles) {
  this->obstacles = &obstacles;
}

bool Player::checkNextActionObstacle(const Direction direction) {
  bool isObstacle = false;
  int x = point.x;
  int y = point.y;

  switch (direction) {
  case Direction::kLeft: {
    x = point.x + delta[0].real();
    break;
  }
  case Direction::kDown: {
    y = point.y - delta[1].imag();
    break;
  }
  case Direction::kRight: {
    x = point.x + delta[2].real();
    break;
  }
  case Direction::kUp: {
    y = point.y - delta[3].imag();
    break;
  }
  }

  if (y < 0)
    return true;
  if (y >= _worldSize)
    return true;
  if (x < 0)
    return true;
  if (x >= _worldSize)
    return true;

  for (auto &obstacle : *obstacles) {
    SDL_Point &point = obstacle->getCoordinates();
    if (x == point.x && y == point.y)
      return true;
  }

  return isObstacle;
}

void Player::action(const Direction direction) {
  if (checkNextActionObstacle(direction)) {
    return;
  }

  switch (direction) {
  case Direction::kLeft: {
    int left = delta[0].real();
    point.x = point.x + left;
    break;
  }
  case Direction::kDown: {
    int down = delta[1].imag();
    point.y = point.y - down;
    break;
  }
  case Direction::kRight: {
    int right = delta[2].real();
    point.x = point.x + right;
    break;
  }
  case Direction::kUp: {
    int up = delta[3].imag();
    point.y = point.y - up;
    break;
  }
  }
}

void Player::action() {}

Enemy::Enemy() : Sprite(State::kEnemy){};
Enemy::~Enemy(){};
Enemy::Enemy(Enemy &other) {
  state = other.state;
  point = other.point;
}
Enemy::Enemy(Enemy &&other) {
  state = std::move(other.state);
  point = std::move(other.point);
}
Enemy &Enemy::operator=(Enemy &other) {
  if (this != &other) {
    state = other.state;
    point = other.point;
  }
  return *this;
}
Enemy &Enemy::operator=(Enemy &&other) {
  if (this != &other) {
    state = std::move(other.state);
    point = std::move(other.point);
  }
  return *this;
}

void Enemy::action() {

  // auto solution = Search(states, init, goal); init - Enemy location, goal
  // -Player location

} // upfront finds path and calculate direction

#endif