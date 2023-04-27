#ifndef PLAYER_H
#define PLAYER_H

#include "astar.h"
// auto solution = Search(states, init, goal); init - Enemy location, goal -
// Player location

enum class Direction { kUp, kDown, kLeft, kRight };

class Sprite {
public:
  Sprite(){};
  ~Sprite(){};
  Sprite(const State &_state) : state(_state) {}
  void setCoordinates(SDL_Point &&point) { this->point = point; }

protected:
  State state;
  SDL_Point point; // top, left, counter-clockwise in world-map coordinate 20x20
};

class Movable : public Sprite {
public:
  virtual void action(const Direction direction,
                      const int worldSize) = 0; // pure virtual function
protected:
  Movable(){};
  Movable(const State &_state) : Sprite(_state) {}
};

class Player : public Movable {
public:
  Player();
  ~Player();
  Player(const State &_state) : Movable(_state) {}
  Player(Player &);
  Player(Player &&);
  Player &operator=(Player &);
  Player &operator=(Player &&);

  void action(const Direction direction, const int worldSize) override;
  void up();
  void down();
  void left();
  void right();
};

class Enemy : public Movable {
public:
  Enemy();
  ~Enemy();
  Enemy(const State &_state) : Movable(_state) {}
  Enemy(Enemy &);
  Enemy(Enemy &&);
  Enemy &operator=(Enemy &);
  Enemy &operator=(Enemy &&);

  void action(const Direction direction, const int worldSize) override;
};

Player::Player() : Movable(State::kPlayer){};
Player::~Player(){};
Player::Player(Player &other) {
  state = other.state;
  point = other.point;
};
Player::Player(Player &&other) {
  state = std::move(other.state);
  point = std::move(other.point);
};
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
void Player::action(const Direction direction, const int worldSize) {
  switch (direction) {
  case Direction::kLeft: {
    int left = delta[0].real();
    point.x = (point.x + left >= 0) ? point.x + left : point.x;
    break;
  }
  case Direction::kDown: {
    int down = delta[1].imag();
    point.y = (point.y - down <= worldSize) ? point.y - down : point.y;
    break;
  }
  case Direction::kRight: {
    int right = delta[2].real();
    point.x = (point.x + right <= worldSize) ? point.x + right : point.x;
    break;
  }
  case Direction::kUp: {
    int up = delta[3].imag();
    point.y = (point.y - up >= 0) ? point.y - up : point.y;
    break;
  }
  }
}
//OR below implementation with boundary checks
void Player::up() { point.y--; }
void Player::down() { point.y++; }
void Player::left() { point.x--; }
void Player::right() { point.x++; }

Enemy::Enemy() : Movable(State::kEnemy){};
Enemy::~Enemy(){};
Enemy::Enemy(Enemy &other) {
  state = other.state;
  point = other.point;
};
Enemy::Enemy(Enemy &&other) {
  state = std::move(other.state);
  point = std::move(other.point);
};
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

  // auto solution = Search(states, init, goal);

} // upfront finds path and calculate direction

#endif