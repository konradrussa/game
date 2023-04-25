#ifndef RENDERER_H
#define RENDERER_H

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <assert.h>

#define winWidth 1000
#define winHeight 1000

class GameRenderer {
public:
  GameRenderer();
  ~GameRenderer();

  void render();
  void setStates(std::vector<std::vector<State>> *states);
  void fullscreen(bool desktop);

private:
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_GLContext context;
  SDL_WindowFlags windowFlags;
  std::vector<std::vector<State>> *states;
  int numberOfCells = 0, sizeOfCell = 0, rows = 0, cols = 0;
  SDL_Rect rect;
  void renderRectangle();
};

GameRenderer::GameRenderer() {
  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
                 "Starting Application.");
  windowFlags = SDL_WINDOW_OPENGL;
  window =
      SDL_CreateWindow("Game Test", 0, 0, winWidth, winHeight, windowFlags);
  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  context = SDL_GL_CreateContext(window);
  assert(window);
  assert(renderer);
}

GameRenderer::~GameRenderer() {
  SDL_GL_DeleteContext(context);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void GameRenderer::renderRectangle() {
  rect.x = cols * sizeOfCell;
  rect.y = rows * sizeOfCell;
  rect.h = sizeOfCell;
  rect.w = sizeOfCell;
  SDL_RenderFillRect(renderer, &rect);
}

void GameRenderer::render() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  rows = 0, cols = 0;
  for (auto &stateRow : *states) {
    for (State &state : stateRow) {
      switch (state) {
      case State::kEmpty: {
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        break;
      }
      case State::kEnemy: {
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
        break;
      }
      case State::kFinish: {
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
        break;
      }
      case State::kObstacle: {
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        break;
      }
      case State::kPath: {
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
        break;
      }
      case State::kPlayer: {
        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
        break;
      }
      }
      renderRectangle();
      cols++;
    }
    cols = 0, rows++;
  }
  SDL_RenderPresent(renderer);
  SDL_Delay(1000);
}

void GameRenderer::setStates(std::vector<std::vector<State>> *states) {
  this->states = states;
  numberOfCells = states->size();
  sizeOfCell = int(winHeight / numberOfCells);
}

void GameRenderer::fullscreen(bool desktop) {
  if (desktop) {
    SDL_SetWindowFullscreen(window,
                            windowFlags | SDL_WINDOW_FULLSCREEN_DESKTOP);
  } else {
    SDL_SetWindowFullscreen(window, windowFlags);
  }
}

/*
void windowSize() {
  int w, h;
  SDL_GetWindowSize(this->window, &w, &h);
  glViewport(0, 0, w, h);
}
*/

#endif