#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <SDL2/SDL.h>
#include <algorithm>
#include <condition_variable>
#include <deque>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>

template <class T> class GameController {
public:
  GameController(){};
  ~GameController(){};

  T receive();
  void send(T &&);
  void logging();
  void logging(std::string msg);
  void handleException(std::exception_ptr exPtr);

private:
  std::mutex _mtx;
  std::condition_variable _cond;
  std::deque<T> _queue;
};

template <class T> T GameController<T>::receive() {
  std::unique_lock<std::mutex> uLock(_mtx);
  _cond.wait(uLock, [this] { return !_queue.empty(); });
  T msg = std::move(_queue.back());
  _queue.pop_back();
  return msg;
}

template <class T> void GameController<T>::send(T &&msg) {
  std::lock_guard gLock(_mtx);
  _queue.push_back(std::move(msg));
  _cond.notify_one();
}

template <class T>
void GameController<T>::handleException(std::exception_ptr exPtr) {
  try {
    std::rethrow_exception(exPtr);
  } catch (const std::exception &e) {
    logging(e.what());
  } catch (...) {
    logging("FATAL ERROR !!!");
  }
}

template <class T> void GameController<T>::logging(std::string msg) {
  SDL_Log("Error: %s\n", msg.c_str());
  std::cerr << "ERROR: " << msg << std::endl;
}

template <class T> void GameController<T>::logging() {
  logging(SDL_GetError());
}

#endif