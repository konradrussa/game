#ifndef INTERACTION_H
#define INTERACTION_H

#include <future>

class GameInteraction {
public:
  void userInteraction(std::promise<char> &&_myPromise);

private:
  SDL_Event event;
};

void GameInteraction::userInteraction(std::promise<char> &&_myPromise) {
  SDL_PollEvent(&event);
  if (event.type == SDL_KEYDOWN) {
    switch (event.key.keysym.sym) {
    case SDLK_ESCAPE:
      _myPromise.set_value('0');
      return;
    case 'f':
      _myPromise.set_value('f');
      return;
    case SDLK_UP:
      _myPromise.set_value('u');
      return;
    case SDLK_DOWN:
      _myPromise.set_value('d');
      return;
    case SDLK_LEFT:
      _myPromise.set_value('l');
      return;
    case SDLK_RIGHT:
      _myPromise.set_value('r');
      return;
    default:
      break;
    }
  } else if (event.type == SDL_QUIT) {
    _myPromise.set_value('0');
    return;
  }
  _myPromise.set_value('1');
}

#endif