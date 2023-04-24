#ifndef PLAYER_H
#define PLAYER_H

class Movable {
protected:
  virtual void move() const = 0; //pure virtual function
};

class Player : public Movable {
public:
  Player();
  ~Player();
  Player(Player &);
  Player(Player &&);
  Player &operator=(Player &);
  Player &operator=(Player &&);

protected:
  void move();
};

class Enemy : public Movable {
public:
  Enemy();
  ~Enemy();
  Enemy(Enemy &);
  Enemy(Enemy &&);
  Enemy &operator=(Enemy &);
  Enemy &operator=(Enemy &&);

protected:
  void move();
};

Player::Player(){};
Player::~Player(){};
Player::Player(Player &other){};
Player::Player(Player &&other){};
Player &Player::operator=(Player &other) { return *this; }
Player &Player::operator=(Player &&other) { return *this; }
void Player::move() {}

Enemy::Enemy(){};
Enemy::~Enemy(){};
Enemy::Enemy(Enemy &other){};
Enemy::Enemy(Enemy &&other){};
Enemy &Enemy::operator=(Enemy &other) { return *this; }
Enemy &Enemy::operator=(Enemy &&other) { return *this; }
void Enemy::move() {}

#endif