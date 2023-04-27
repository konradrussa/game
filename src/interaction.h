#ifndef INTERACTION_H
#define INTERACTION_H

#include "player.h"
#include "renderer.h"

class GameInteraction {
public:
  int userInteraction(std::unique_ptr<GameRenderer> &gRenderer, Player &player);

private:
  SDL_Event event;
};

int GameInteraction::userInteraction(std::unique_ptr<GameRenderer> &gRenderer,
                                     Player &player) {
  SDL_PollEvent(&event);
  if (event.type == SDL_KEYDOWN) {
    switch (event.key.keysym.sym) {
    case SDLK_ESCAPE:
      return 0;
    case 'f':
      gRenderer->fullscreen();
      break;
    case SDLK_UP:
      player.up();
      // update map
      // send signal to enemy
      break;
    case SDLK_DOWN:
      player.down();
      // update map
      // send signal to enemy
      break;
    case SDLK_LEFT:
      player.left();
      // update map
      // send signal to enemy
      break;
    case SDLK_RIGHT:
      player.right();
      // update map
      // send signal to enemy
      break;
    default:
      break;
    }
  } else if (event.type == SDL_QUIT) {
    return 0;
  }
  return 1;
}

#endif