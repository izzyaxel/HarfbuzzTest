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
          vec2<float> charPos = text.penPositions.at(i);
          
          for(const auto& effect : text.effects)
          {
            switch(effect)
            {
              case TextEffect::JITTER:
              {
                if(this->frames % text.jitterUpdateRate == 0)
                {
                  text.currentJitter.at(i) = {randomFloat(-text.jitterAmount, text.jitterAmount), randomFloat(-text.jitterAmount, text.jitterAmount)};
                }
                break;
              }
              case TextEffect::RAINBOW:
              {
                if(this->frames % text.rainbowUpdateRate == 0)
                {
                  text.currentColor.at(i).fromRGBAf(randomFloat(), randomFloat(), randomFloat(), 1);
                }
                break;
              }
              case TextEffect::SOLID_RAINBOW:
              {
                if(this->frames % text.rainbowUpdateRate == 0)
                {
                  vec3 color = {randomFloat(), randomFloat(), randomFloat()};
                  for(auto& c : text.currentColor)
                  {
                    c.fromRGBAf(color.r(), color.g(), color.b(), 1);
                  }
                }
                break;
              }
              case TextEffect::SOLID_RAINBOW_FADE:
              {
                
                break;
              }
              default: break;
            }
          }

          charPos += text.currentJitter.at(i);
          const char& character = text.text[i];
          vec2 size = text.atlas->getTileDimensions(std::string{character});
          glr::QuadUVs uvs = text.atlas->getUVsForTile(std::string{character});
        
          glr::Renderable r{{charPos.x() - 200, charPos.y(), 0.0f}, {size.width(), size.height(), 1}, quat<float>{},
          &*text.texture,
          &textShader,
          &quad,
          1, 0, "text",
          glr::Renderable::CharacterInfo{character, text.currentColor.at(i), uvs, "inputColor"}};
        
          rl.add({r});
        }
      }
      this->window.draw(std::move(rl));
      this->window.swapFramebuffer();
      this->frames++;
    }
  }
}
