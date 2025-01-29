#include "application.hh"
#include "shaders.hh"

#include <SDL2/SDL.h>
#include <glrRenderList.hh>

glr::Shader objectShader;
glr::Shader textShader;
glr::Mesh fullscreenQuad;
glr::Mesh quad;

Application::Application(const u32 width, const u32 height) : window(width, height)
{
  objectShader = glr::Shader("Object shader", vertSrc, fragSrc);
  textShader = glr::Shader("Text shader", vertSrc, textFragSrc);

  const std::vector<float> orthoQuadUVs{1, 0, 0, 0, 1, 1, 0, 1};
  const std::vector<float> fullscreenQuadUVs{1, 0, 1, 1, 0, 0, 0, 1};
  const std::vector<float> fullscreenQuadVerts{1, -1, 0, 1, 1, 0, -1, -1, 0, -1, 1, 0};
  const std::vector<float> centeredQuadVerts{0.5f, 0.5f, 0, -0.5f, 0.5f, 0, 0.5f, -0.5f, 0, -0.5f, -0.5f, 0};
  const std::vector<float> llQuadVerts{1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0};
  const std::vector<float> lrQuadVerts{0, 1, 0, -1, 1, 0, 0, 0, 0, -1, 0, 0};
  const std::vector<float> ulQuadVerts{1, 0, 0, 0, 0, 0, 1, -1, 0, 0, -1, 0};
  const std::vector<float> urQuadVerts{0, 0, 0, -1, 0, 0, 0, -1, 0, -1, -1, 0};
  
  fullscreenQuad = glr::Mesh(fullscreenQuadVerts, fullscreenQuadUVs);
  quad = glr::Mesh(centeredQuadVerts, orthoQuadUVs);
}

void Application::addText(const Text& text)
{
  this->textToRender.emplace_back(text);
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

    glr::RenderList rl;
    for(const auto& text : this->textToRender)
    {
      glr::Renderable r{{0.0f, 0.0f, 0.0f}, {800, 600, 1}, quat<float>{},
      &this->fontRasterizer.getFontData(text.fontID).texture,
      &objectShader,
      &fullscreenQuad,
      1, 0, "text"
      };
      rl.add({r});
    }
    this->window.draw(std::move(rl));
    this->window.swapFramebuffer();
    SDL_Delay(16); //Roughly 60hz refresh
  }
}
