#ifndef PLAYER_H
#define PLAYER_H

#include "astar.h"

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
  virtual void action(const Direction direction,
                      const int worldSize) = 0; // pure virtual function
};

class Player : public Sprite, public Movable {
public:
  Player();
  ~Player();
  Player(const State &_state) : Sprite(_state) {}
  Player(Player &);
  Player(Player &&);
  Player &operator=(Player &);
  Player &operator=(Player &&);

  void action(const Direction direction, const int worldSize) override;
  void setObstacles(std::vector<std::shared_ptr<Sprite>> &obstacles);
  bool checkNextActionObstacle(const Direction direction, const int worldSize);

protected:
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

  void action(const Direction direction, const int worldSize) override;
};

Player::Player() : Sprite(State::kPlayer){};
Player::~Player(){};
Player::Player(Player &other) {
  state = other.state;
  point = other.point;
}
Player::Player(Player &&other) {
  state = std::move(other.state);
  point = std::move(other.point);
}
Player &Player::operator=(Player &other) {
  if (this != &other) {
    state = other.state;
    point = other.point;
  }
  return *this;
}
Player &Player::operator=(Player &&other) {
  if (this != &other) {
    state = std::move(other.state);
    point = std::move(other.point);
  }
  return *this;
}

void Player::setObstacles(std::vector<std::shared_ptr<Sprite>> &obstacles) {
  this->obstacles = &obstacles;
}

bool Player::checkNextActionObstacle(const Direction direction,
                                     const int worldSize) {
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
  if (y >= worldSize)
    return true;
  if (x < 0)
    return true;
  if (x >= worldSize)
    return true;

  for (auto &obstacle : *obstacles) {
    SDL_Point &point = obstacle->getCoordinates();
    if (x == point.x && y == point.y)
      return true;
  }

  return isObstacle;
}

void Player::action(const Direction direction, const int worldSize) {
  if (checkNextActionObstacle(direction, worldSize)) {
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

void Enemy::action(const Direction direction, const int worldSize) {

  // auto solution = Search(states, init, goal); init - Enemy location, goal
  // -Player location

} // upfront finds path and calculate direction

#endif