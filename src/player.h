#ifndef PLAYER_H
#define PLAYER_H

enum class Direction { kUp, kDown, kLeft, kRight };

class Sprite {
public:
  Sprite(){};
  ~Sprite(){};
  Sprite(const State &_state) : state(_state) {}
  void setCoordinates(SDL_Point &&point) { this->point = point; }

protected:
  State state;
  SDL_Point point; // top, left, counter-clockwise
};

class Movable : public Sprite {
protected:
  Movable(){};
  Movable(const State &_state) : Sprite(_state) {}
  virtual void action(Direction direction) const = 0; // pure virtual function
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

  void action(Direction direction);
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

  void action(Direction direction);
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
void Player::action(Direction direction) {}

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
void Enemy::action(Direction direction) {}

#endif