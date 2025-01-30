#include "application.hh"
#include "shaders.hh"

#include <SDL2/SDL.h>
#include <glrRenderList.hh>

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

Application::Application(const u32 width, const u32 height) : window(width, height)
{
  objectShader = glr::Shader("Object shader", vertSrc, fragSrc);
  textShader = glr::Shader("Text shader", vertSrc, textFragSrc);
  
  fullscreenQuad = glr::Mesh(fullscreenQuadVerts, fullscreenQuadUVs);
  quad = glr::Mesh(ulQuadVerts, orthoQuadUVs);
}

void Application::addText(const Text& text)
{
  this->textToRender.emplace_back(text);
}

void Application::run()
{
  while(!this->exiting)
  {
    //std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
    
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
      FontData& fontData = this->fontRasterizer.getFontData(text.fontID);
      for(size_t i = 0; i < text.text.size(); i++)
      {
        glr::Color charColor;
        charColor.fromRGBAf(randomFloat(), randomFloat(), randomFloat(), 1);
        
        const char& character = text.text[i];
        vec2 pos = text.penPositions.at(i);
        vec2 size = fontData.atlas.getTileDimensions(std::string{character});
        glr::QuadUVs uvs = fontData.atlas.getUVsForTile(std::string{character});
        
        glr::Renderable r{{pos.x() - 200, pos.y(), 0.0f}, {size.width(), size.height(), 1}, quat<float>{},
        &this->fontRasterizer.getFontData(text.fontID).texture,
        &textShader,
        &quad,
        1, 0, "text",
        glr::Renderable::CharacterInfo{character, charColor, uvs, "inputColor"}};
        
        rl.add({r});
      }
    }
    this->window.draw(std::move(rl));
    this->window.swapFramebuffer();
    SDL_Delay(14);
    //std::chrono::duration<float> elapsed = std::chrono::high_resolution_clock::now() - begin;
    //printf("%f fps\n", 1.0f / elapsed.count());
  }
}
