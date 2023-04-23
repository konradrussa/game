#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <SDL.h>
#include <algorithm>
#include <condition_variable>
#include <deque>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>

class GameController {
public:
  GameController(){};
  ~GameController(){};

  void logging();
  void logging(std::string msg);
  void handleException(std::exception_ptr exPtr);
};

void GameController::handleException(std::exception_ptr exPtr) {
  try {
    std::rethrow_exception(exPtr);
  } catch (const std::exception &e) {
    logging(e.what());
  } catch (...) {
    logging("FATAL ERROR !!!");
  }
}

void GameController::logging(std::string msg) {
  SDL_Log("Error: %s\n", msg.c_str());
  std::cerr << "ERROR: " << msg << std::endl;
}

void GameController::logging() { logging(SDL_GetError()); }

#endif