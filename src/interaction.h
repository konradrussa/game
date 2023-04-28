#ifndef INTERACTION_H
#define INTERACTION_H

#include <future>

class GameInteraction {
public:
  void userInteraction(std::promise<char> &&myPromise);

private:
  SDL_Event event;
};

void GameInteraction::userInteraction(std::promise<char> &&myPromise) {
  SDL_PollEvent(&event);
  if (event.type == SDL_KEYDOWN) {
    switch (event.key.keysym.sym) {
    case SDLK_ESCAPE:
      myPromise.set_value('0');
      return;
    case 'f':
      myPromise.set_value('f');
      return;
    case SDLK_UP:
      myPromise.set_value('u');
      return;
    case SDLK_DOWN:
      myPromise.set_value('d');
      return;
    case SDLK_LEFT:
      myPromise.set_value('l');
      return;
    case SDLK_RIGHT:
      myPromise.set_value('r');
      return;
    default:
      break;
    }
  } else if (event.type == SDL_QUIT) {
    myPromise.set_value('0');
    return;
  }
  myPromise.set_value('1');
}

#endif