#include <GL/gl.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <assert.h>
#include <chrono>
#include <stdint.h>
#include <stdio.h>
#include <vector>

#define winWidth 1000
#define winHeight 1000

bool init_resources(void) { return true; }

void render(SDL_Renderer &renderer) {}

void free_resources() {}

SDL_Renderer &mainLoop(SDL_Window &window, SDL_WindowFlags windowFlags) {

  SDL_Renderer *renderer = SDL_CreateRenderer(
      &window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  SDL_Event event;
  int running = 1;
  int fullScreen = 0;

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
          running = 0;
          break;
        case 'f':
          fullScreen = !fullScreen;
          if (fullScreen) {
            SDL_SetWindowFullscreen(&window, windowFlags |
                                                 SDL_WINDOW_FULLSCREEN_DESKTOP);
          } else {
            SDL_SetWindowFullscreen(&window, windowFlags);
          }
          break;
        default:
          break;
        }
      } else if (event.type == SDL_QUIT) {
        running = 0;
      }
    }
    render(*renderer);
  }
  return *renderer;
}

int main(int argCount, char **args) {
  auto windowFlags = SDL_WINDOW_OPENGL;
  SDL_Window *window =
      SDL_CreateWindow("OpenGL Test", 0, 0, winWidth, winHeight, windowFlags);
  assert(window);
  // SDL_GLContext Context = SDL_GL_CreateContext(window);

  if (!init_resources())
    return EXIT_FAILURE;

  SDL_Renderer &renderer = mainLoop(*window, windowFlags);

  SDL_DestroyRenderer(&renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}