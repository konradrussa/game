#ifndef RENDERER_H
#define RENDERER_H

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <assert.h>
#include <vector>

#define winWidth 1000
#define winHeight 1000

// forward declaration to avoid include cycles
class Sprint;

class GameRenderer {
public:
  GameRenderer();
  ~GameRenderer();

  void setObstaclesAndFinish(int numberOfCells,
                             std::vector<std::shared_ptr<Sprite>> &obstacles,
                             std::shared_ptr<Sprite> &finish);
  void render(std::shared_ptr<Player> &player, std::shared_ptr<Enemy> &enemy);
  void renderFinish();
  void fullscreen();

  int getWorldSize() { return _numberOfCells; } // printed window

private:
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_GLContext context;
  SDL_WindowFlags windowFlags;
  int _numberOfCells = 0, _sizeOfCell = 0;
  SDL_Rect rect;
  bool _fullScreen = false;
  std::vector<std::shared_ptr<Sprite>> *obstacles;
  std::shared_ptr<Sprite> *finish;
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

void GameRenderer::setObstaclesAndFinish(
    int numberOfCells, std::vector<std::shared_ptr<Sprite>> &obstacles,
    std::shared_ptr<Sprite> &finish) {
  this->obstacles = &obstacles;
  this->finish = &finish;
  _numberOfCells = numberOfCells;
  _sizeOfCell = int(winHeight / _numberOfCells);
}

void GameRenderer::render(std::shared_ptr<Player> &player,
                          std::shared_ptr<Enemy> &enemy) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  // common size
  rect.h = _sizeOfCell;
  rect.w = _sizeOfCell;

  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
  SDL_Point &finishPoint = (*finish)->getCoordinates();
  rect.x = finishPoint.x * _sizeOfCell;
  rect.y = finishPoint.y * _sizeOfCell;
  SDL_RenderFillRect(renderer, &rect);

  for (std::shared_ptr<Sprite> &obstacle : *obstacles) {
    SDL_Point &obstaclePoint = obstacle->getCoordinates();
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    rect.x = obstaclePoint.x * _sizeOfCell;
    rect.y = obstaclePoint.y * _sizeOfCell;
    SDL_RenderFillRect(renderer, &rect);
  }

  SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
  SDL_Point &playerPoint = player->getCoordinates();
  rect.x = playerPoint.x * _sizeOfCell;
  rect.y = playerPoint.y * _sizeOfCell;
  SDL_RenderFillRect(renderer, &rect);

  SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
  SDL_Point &enemyPoint = enemy->getCoordinates();
  rect.x = enemyPoint.x * _sizeOfCell;
  rect.y = enemyPoint.y * _sizeOfCell;
  SDL_RenderFillRect(renderer, &rect);

  SDL_RenderPresent(renderer);
}

void GameRenderer::fullscreen() {
  _fullScreen = !_fullScreen;
  if (_fullScreen) {
    SDL_SetWindowFullscreen(window,
                            windowFlags | SDL_WINDOW_FULLSCREEN_DESKTOP);
  } else {
    SDL_SetWindowFullscreen(window, windowFlags);
  }
}

void GameRenderer::renderFinish() {
  const std::vector<SDL_Vertex> verts = {
      {
          SDL_FPoint{500, 250},
          SDL_Color{255, 0, 0, 255},
          SDL_FPoint{0},
      },
      {
          SDL_FPoint{250, 750},
          SDL_Color{0, 0, 255, 255},
          SDL_FPoint{0},
      },
      {
          SDL_FPoint{750, 750},
          SDL_Color{0, 255, 0, 255},
          SDL_FPoint{0},
      },
  };
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  SDL_RenderGeometry(renderer, nullptr, verts.data(), verts.size(), nullptr, 0);
  SDL_RenderPresent(renderer);
}

/*
void windowSize() {
  int w, h;
  SDL_GetWindowSize(this->window, &w, &h);
  glViewport(0, 0, w, h);
}
*/

#endif