#include "openGl.hh"

#include <SDL2/SDL.h>

SDL_Window* window;
void* context;

void startOpenGL()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
  if(!window)
  {
    throw std::runtime_error("Failed to create a window");
  }
  SDL_GL_LoadLibrary(nullptr);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  context = SDL_GL_CreateContext(window);
  if(!context)
  {
    throw std::runtime_error("SDL failed to create an OpenGL context");
  }
  SDL_GL_SetSwapInterval(0);
  SDL_GL_SwapWindow(window);
}
