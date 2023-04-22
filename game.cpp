#include <GL/gl.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <assert.h>
#include <chrono>
#include <stdint.h>
#include <stdio.h>
#include <vector>

typedef int32_t i32;
typedef uint32_t u32;
typedef int32_t b32;

#define WinWidth 1000
#define WinHeight 1000

int main(int ArgCount, char **Args) {

  u32 WindowFlags = SDL_WINDOW_OPENGL;
  SDL_Window *Window =
      SDL_CreateWindow("OpenGL Test", 0, 0, WinWidth, WinHeight, WindowFlags);
  assert(Window);
  //SDL_GLContext Context = SDL_GL_CreateContext(Window);

  SDL_Renderer *renderer = SDL_CreateRenderer(
      Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

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

  b32 Running = 1;
  b32 FullScreen = 0;
  while (Running) {
    SDL_Event Event;
    while (SDL_PollEvent(&Event)) {
      if (Event.type == SDL_KEYDOWN) {
        switch (Event.key.keysym.sym) {
        case SDLK_ESCAPE:
          Running = 0;
          break;
        case 'f':
          FullScreen = !FullScreen;
          if (FullScreen) {
            SDL_SetWindowFullscreen(Window, WindowFlags |
                                                SDL_WINDOW_FULLSCREEN_DESKTOP);
          } else {
            SDL_SetWindowFullscreen(Window, WindowFlags);
          }
          break;
        default:
          break;
        }
      } else if (Event.type == SDL_QUIT) {
        Running = 0;
      }
    }

    glViewport(0, 0, WinWidth, WinHeight);
    glClearColor(1.f, 0.f, 1.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(Window);
    /*
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        SDL_RenderGeometry(renderer, nullptr, verts.data(), verts.size(),
       nullptr,0); SDL_RenderPresent(renderer);
    */
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(Window);
  SDL_Quit();
  return 0;
}