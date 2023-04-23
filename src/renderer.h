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
  void fullscreen(bool desktop);

private:
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_WindowFlags windowFlags;
  std::vector<SDL_Vertex> verts;
  std::vector<SDL_Point> points;
  void renderStart(SDL_Rect &rect);
};

GameRenderer::GameRenderer() {
  windowFlags = SDL_WINDOW_OPENGL;
  window =
      SDL_CreateWindow("Game Test", 0, 0, winWidth, winHeight, windowFlags);
  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  // SDL_GLContext context = SDL_GL_CreateContext(game.window);
  assert(window);
  assert(renderer);
  verts = {{
               SDL_FPoint{400, 150},
               SDL_Color{255, 0, 0, 255},
               SDL_FPoint{0},
           },
           {
               SDL_FPoint{200, 450},
               SDL_Color{0, 0, 255, 255},
               SDL_FPoint{0},
           },
           {
               SDL_FPoint{600, 450},
               SDL_Color{0, 255, 0, 255},
               SDL_FPoint{0},
           }};
}

GameRenderer::~GameRenderer() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void GameRenderer::renderStart(SDL_Rect &rect) {
  SDL_SetRenderDrawColor(renderer, 0xFF, 0xCC, 0x00, 0xFF);
  rect.h = 8;
  rect.w = 8;
  SDL_RenderFillRect(renderer, &rect);
}

void GameRenderer::render() {

  // glViewport(0, 0, winWidth, winHeight);
  // glClearColor(1.f, 0.f, 1.f, 0.f);
  // glClear(GL_COLOR_BUFFER_BIT);
  // GLuint colorbuffer;
  // SDL_GL_SwapWindow(&window);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  SDL_RenderGeometry(renderer, nullptr, verts.data(), verts.size(), nullptr, 0);

  SDL_Rect rect;
  rect.x = 100;
  rect.y = 50;

  renderStart(rect);

  SDL_RenderPresent(renderer);
}

void GameRenderer::fullscreen(bool desktop) {
  if (desktop) {
    SDL_SetWindowFullscreen(window,
                            windowFlags | SDL_WINDOW_FULLSCREEN_DESKTOP);
  } else {
    SDL_SetWindowFullscreen(window, windowFlags);
  }
}

#endif