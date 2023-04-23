#ifndef MAP_H
#define MAP_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define MAP_FILE "data/map.txt"

enum class State { kEmpty, kEnemy, kObstacle, kPath, kStart, kFinish };

class GameMap {
public:
  GameMap(){};
  ~GameMap(){};

  bool readMap();
  std::vector<std::vector<State>> &getStates();

private:
  std::vector<std::vector<State>> states;
  std::vector<State> parseLine(std::string line);
};

std::vector<State> GameMap::parseLine(std::string line) {
  std::istringstream sline(line);
  char n, c;
  std::vector<State> row;
  while (sline >> n >> c && c == ',') {
    if (n == '0') {
      row.push_back(State::kEmpty);
    } else if (n == '1') {
      row.push_back(State::kObstacle);
    } else if (n == 'S') {
      row.push_back(State::kStart);
    } else if (n == 'X') {
      row.push_back(State::kFinish);
    } else if (n == 'E') {
      row.push_back(State::kEnemy);
    } else {
      throw std::runtime_error("Unknown character on map!");
    }
  }
  return row;
}

bool GameMap::readMap() {
  std::ifstream myFile(MAP_FILE);
  if (myFile) {
    std::string line;
    while (getline(myFile, line)) {
      std::vector<State> row = parseLine(line);
      states.push_back(row);
    }
  } else {
    throw std::runtime_error("Map file not found!");
  }
  return true;
}

std::vector<std::vector<State>> &GameMap::getStates() { return states; }

#endif