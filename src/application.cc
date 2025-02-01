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

void Application::addTextToRender(const ID text)
{
  this->textToRender.emplace_back(text);
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

      //Render text
      glr::RenderList rl;
      for(const ID entity : this->textToRender)
      {
        TextBlock& text = this->textECS.getText(entity);

        if(this->textECS.hasSolidRainbowEffect(entity))
        {
          SolidRainbowEffect& effect = this->textECS.getSolidRainbowEffect(entity);
          if(this->frames % effect.updateRate == 0)
          {
            effect.apply(text.currentColor, this->deltaTime.deltaF);
          }
        }
        if(this->textECS.hasSolidRainbowFadeEffect(entity))
        {
          SolidRainbowFadeEffect& effect = this->textECS.getSolidRainbowFadeEffect(entity);
          if(this->frames % effect.updateRate == 0)
          {
            effect.apply(text.currentColor, this->deltaTime.deltaF);
          }
        }
        
        for(size_t i = 0; i < text.text.size(); i++)
        {
          vec2<float> charPos = text.penPositions.at(i);

          if(this->textECS.hasJitterEffect(entity))
          {
            JitterEffect& effect = this->textECS.getJitterEffect(entity);
            if(this->frames % effect.updateRate == 0)
            {
              effect.apply(i, charPos, this->deltaTime.deltaF);
            }
          }
          if(this->textECS.hasRainbowEffect(entity))
          {
            RainbowEffect& effect = this->textECS.getRainbowEffect(entity);
            if(this->frames % effect.updateRate == 0)
            {
              effect.apply(i, text.currentColor, this->deltaTime.deltaF);
            }
          }

          //TODO FIXME position/color data persistence when effects are active but updateRate is not 1
          const char& character = text.text.at(i);
          vec2 size = text.atlas->getTileDimensions(std::string{character});
          glr::QuadUVs uvs = text.atlas->getUVsForTile(std::string{character});
        
          glr::Renderable r{{text.pos.x() + charPos.x(), text.pos.y() + charPos.y(), 0.0f}, {size.width(), size.height(), 1}, quat<float>{},
          &*text.texture,
          &textShader,
          &quad,
          1, 0, "text",
          glr::Renderable::CharacterInfo{character, text.currentColor, uvs, "inputColor"}};
          
          rl.add({r});
        }
      }
      this->window.draw(std::move(rl));
      this->window.swapFramebuffer();
      this->frames++;
    }
  }
}
