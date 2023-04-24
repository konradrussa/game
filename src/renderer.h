#ifndef RENDERER_H
#define RENDERER_H

#include <GL/gl.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <assert.h>

#define winWidth 1000
#define winHeight 1000

class GameRenderer {
public:
  GameRenderer();
  ~GameRenderer();

  void render();
  void loadMap(std::vector<std::vector<State>> *states);
  void fullscreen(bool desktop);

private:
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_GLContext context;
  SDL_WindowFlags windowFlags;
  // std::vector<SDL_Point> points;
  std::vector<std::vector<State>> *states;

  void renderRect(SDL_Rect &rect, int sizeOfCell);
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

void GameRenderer::renderRect(SDL_Rect &rect, int sizeOfCell) {
  rect.h = sizeOfCell;
  rect.w = sizeOfCell;
  SDL_RenderFillRect(renderer, &rect);
}

void GameRenderer::render() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  int numberOfCells = states->size();
  int sizeOfCell = int(winHeight / numberOfCells);
  int x = 0, y = 0;
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
      case State::kStart: {
        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
        break;
      }
      }
      SDL_Rect rect;
      rect.x = x * sizeOfCell;
      rect.y = y * sizeOfCell;
      renderRect(rect, sizeOfCell);
      x++;
    }
    x = 0;
    y++;
  }
  SDL_RenderPresent(renderer);
}

void GameRenderer::loadMap(std::vector<std::vector<State>> *states) {
  this->states = states;
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