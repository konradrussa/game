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

class Controller {
public:
  Controller(){};
  ~Controller(){};
  void handleException(std::exception_ptr exPtr);
  void logging();
  void logging(std::string msg);
};

void Controller::handleException(std::exception_ptr exPtr) {
  try {
    std::rethrow_exception(exPtr);
  } catch (const std::exception &e) {
    logging(e.what());
  } catch (...) {
    logging("FATAL ERROR !!!");
  }
}
void Controller::logging(std::string msg) {
  SDL_Log("Error: %s\n", msg.c_str());
  std::cerr << "ERROR: " << msg << std::endl;
}
void Controller::logging() { SDL_Log("SDL_Error: %s\n", SDL_GetError()); }

#endif