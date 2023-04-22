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

int main(int argCount, char **args) {

  auto windowFlags = SDL_WINDOW_OPENGL;
  SDL_Window *window =
      SDL_CreateWindow("OpenGL Test", 0, 0, winWidth, winHeight, windowFlags);
  assert(window);
  // SDL_GLContext Context = SDL_GL_CreateContext(Window);

  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  const std::vector<SDL_Vertex> verts = {
      {
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
      },
  };

  int running = 1;
  int fullScreen = 0;
  SDL_Event event;
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
            SDL_SetWindowFullscreen(window, windowFlags |
                                                SDL_WINDOW_FULLSCREEN_DESKTOP);
          } else {
            SDL_SetWindowFullscreen(window, windowFlags);
          }
          break;
        default:
          break;
        }
      } else if (event.type == SDL_QUIT) {
        running = 0;
      }
    }

    glViewport(0, 0, winWidth, winHeight);
    glClearColor(1.f, 0.f, 1.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderGeometry(renderer, nullptr, verts.data(), verts.size(), nullptr,
                       0);
    SDL_RenderPresent(renderer);

    SDL_GL_SwapWindow(window);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}