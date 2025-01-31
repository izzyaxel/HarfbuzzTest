#include "application.hh"
#include "shaders.hh"

#include <SDL2/SDL.h>
#include <glrRenderList.hh>

#include "util.hh"

glr::Shader objectShader;
glr::Shader textShader;

glr::Mesh fullscreenQuad;
glr::Mesh quad;

constexpr std::array<float, 8>  fullscreenQuadUVs{1, 0, 1, 1, 0, 0, 0, 1};
constexpr std::array<float, 12> fullscreenQuadVerts{1, -1, 0, 1, 1, 0, -1, -1, 0, -1, 1, 0};
constexpr std::array<float, 8>  orthoQuadUVs{1, 0, 0, 0, 1, 1, 0, 1};
constexpr std::array<float, 12> cQuadVerts{0.5f, 0.5f, 0, -0.5f, 0.5f, 0, 0.5f, -0.5f, 0, -0.5f, -0.5f, 0};
constexpr std::array<float, 12> llQuadVerts{1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0};
constexpr std::array<float, 12> lrQuadVerts{0, 1, 0, -1, 1, 0, 0, 0, 0, -1, 0, 0};
constexpr std::array<float, 12> ulQuadVerts{1, 0, 0, 0, 0, 0, 1, -1, 0, 0, -1, 0};
constexpr std::array<float, 12> urQuadVerts{0, 0, 0, -1, 0, 0, 0, -1, 0, -1, -1, 0};

Application::Application(const u32 width, const u32 height, const u32 targetFPS) : window(width, height)
{
  this->targetFPS = targetFPS;
  this->deltaTime.setTargetFinder([this]{return 1.0f / (float)this->targetFPS;});
  
  objectShader = glr::Shader("Object shader", vertSrc, fragSrc);
  textShader = glr::Shader("Text shader", vertSrc, textFragSrc);
  
  fullscreenQuad = glr::Mesh(fullscreenQuadVerts, fullscreenQuadUVs);
  quad = glr::Mesh(ulQuadVerts, orthoQuadUVs);
}

void Application::addText(TextBlock text)
{
  this->textToRender.emplace_back(std::move(text));
}

void Application::run()
{
  while(!this->exiting)
  {
    this->deltaTime.update();
    if(this->deltaTime.isTargetReached())
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
      for(auto& text : this->textToRender)
      {
        for(size_t i = 0; i < text.text.size(); i++)
        {
          //Apply effects
          vec2<float> charPos = text.penPositions.at(i);
          glr::Color charColor = text.color;
          
          for(const auto& effect : text.effects)
          {
            if(this->frames % effect->updateRate == 0)
            {
              effect->apply(i, charPos, charColor);
            }
          }

          //Construct a renderable for the glyph
          const char& character = text.text.at(i);
          vec2 size = text.atlas->getTileDimensions(std::string{character});
          glr::QuadUVs uvs = text.atlas->getUVsForTile(std::string{character});
        
          glr::Renderable r{{text.pos.x() + charPos.x(), text.pos.y() + charPos.y(), 0.0f}, {size.width(), size.height(), 1}, quat<float>{},
          &*text.texture,
          &textShader,
          &quad,
          1, 0, "text",
          glr::Renderable::CharacterInfo{character, charColor, uvs, "inputColor"}};
        
          rl.add({r});
        }
      }
      this->window.draw(std::move(rl));
      this->window.swapFramebuffer();
      this->frames++;
    }
  }
}
