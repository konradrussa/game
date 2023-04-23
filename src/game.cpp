#include "game.h"

bool Game::initResources() {
  try {
    gameController->logging("Reading MAP");
    return gameMap->readMap();
  } catch (...) {
    gameController->handleException(std::current_exception());
  }
  return false;
}

void Game::freeResources() {}

void Game::mainLoop() {

  SDL_Event event;
  int running = 1;
  bool fullScreen = false;

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
          running = 0;
          break;
        case 'f':
          fullScreen = !fullScreen;
          gameRenderer->fullscreen(fullScreen);
          break;
        default:
          break;
        }
      } else if (event.type == SDL_QUIT) {
        running = 0;
      }
    }
    gameRenderer->render();
  }
}

int main(int argCount, char **args) {
  Game game;

  if (!game.initResources()) {
    return EXIT_FAILURE;
  }

  game.mainLoop();

  return EXIT_SUCCESS;
}