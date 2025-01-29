#include "window.hh"

#include <SDL2/SDL.h>
#include <glrRenderer.hh>

Window::Window(const u32 width, const u32 height)
{
  SDL_Init(SDL_INIT_EVERYTHING);
  this->window = SDL_CreateWindow("Harfbuzz Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
  if(!this->window)
  {
    throw std::runtime_error("Failed to create a window");
  }
  SDL_GL_LoadLibrary(nullptr);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  
  this->context = SDL_GL_CreateContext(static_cast<SDL_Window*>(this->window));
  if(!this->context)
  {
    throw std::runtime_error("SDL failed to create an OpenGL context");
  }
  
  SDL_GL_SetSwapInterval(0);

  this->renderer = new glr::Renderer(reinterpret_cast<glr::GLLoadFunc>(SDL_GL_GetProcAddress), width, height);
  
  glr::Color color;
  color.fromRGBAf(0.5f, 0.3f, 0.8f, 1.0f);
  static_cast<glr::Renderer*>(this->renderer)->setClearColor(color);
}

Window::~Window()
{
  delete static_cast<glr::Renderer*>(this->renderer);
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(static_cast<SDL_Window*>(window));
  SDL_Quit();
}

void Window::clearFramebuffer() const
{
  static_cast<glr::Renderer*>(this->renderer)->clearCurrentFramebuffer();
}

void Window::swapFramebuffer() const
{
  SDL_GL_SwapWindow(static_cast<SDL_Window*>(this->window));
}


