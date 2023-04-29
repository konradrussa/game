#ifndef RENDERER_H
#define RENDERER_H

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <assert.h>
#include <vector>

#include "map.h"

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
  void fullscreen();

  int getWorldSize() { return winWidth; } // printed window

private:
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_GLContext context;
  SDL_WindowFlags windowFlags;
  int numberOfCells = 0, sizeOfCell = 0;
  SDL_Rect rect;
  bool fullScreen = false;
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
  numberOfCells = numberOfCells;
  sizeOfCell = int(winHeight / numberOfCells);
}

void GameRenderer::render(std::shared_ptr<Player> &player,
                          std::shared_ptr<Enemy> &enemy) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  // common size
  rect.h = sizeOfCell;
  rect.w = sizeOfCell;

  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
  SDL_Point &finishPoint = (*finish)->getCoordinates();
  rect.x = finishPoint.x * sizeOfCell;
  rect.y = finishPoint.y * sizeOfCell;
  SDL_RenderFillRect(renderer, &rect);

  for (std::shared_ptr<Sprite> &obstacle : *obstacles) {
    SDL_Point &obstaclePoint = obstacle->getCoordinates();
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    rect.x = obstaclePoint.x * sizeOfCell;
    rect.y = obstaclePoint.y * sizeOfCell;
    SDL_RenderFillRect(renderer, &rect);
  }

  SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
  SDL_Point &playerPoint = player->getCoordinates();
  rect.x = playerPoint.x * sizeOfCell;
  rect.y = playerPoint.y * sizeOfCell;
  SDL_RenderFillRect(renderer, &rect);

  SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
  SDL_Point &enemyPoint = enemy->getCoordinates();
  rect.x = enemyPoint.x * sizeOfCell;
  rect.y = enemyPoint.y * sizeOfCell;
  SDL_RenderFillRect(renderer, &rect);

  SDL_RenderPresent(renderer);
}

void GameRenderer::fullscreen() {
  fullScreen = !fullScreen;
  if (fullScreen) {
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