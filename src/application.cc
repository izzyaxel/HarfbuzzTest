#include "application.hh"

#include <SDL2/SDL.h>

Application::Application(const u32 width, const u32 height) : window(width, height)
{
  
}

void Application::run()
{
  
  while(!this->exiting)
  {
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
      switch(event.type)
      {
        case SDL_QUIT: //Make the menu bar x button close the window
        {
          this->exiting = true;
          break;
        }
        case SDL_KEYUP:
        {
          if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
          {
            this->exiting = true;
          }
          break;
        }
        default: break;
      }
    }
    this->window.clearFramebuffer();
    this->window.swapFramebuffer();
    SDL_Delay(16); //Roughly 60hz refresh
  }
}
