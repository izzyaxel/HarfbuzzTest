#include "textRenderer.hh"

#include "../shaders.hh"

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

TextRenderer::TextRenderer()
{
  objectShader = glr::Shader("Object shader", vertSrc, fragSrc);
  textShader = glr::Shader("Text shader", vertSrc, textFragSrc);
  
  fullscreenQuad = glr::Mesh(fullscreenQuadVerts, fullscreenQuadUVs);
  quad = glr::Mesh(cQuadVerts, orthoQuadUVs);
}

void TextRenderer::addText(ID entity)
{
  this->textToRender.emplace_back(entity);
}

glr::RenderList TextRenderer::makeSceneGraph(const u64 frames, const float deltaTime)
{
  glr::RenderList out;
  for(const ID entity : this->textToRender)
  {
    TextBlock& text = this->ecs.getText(entity);
    
    if(this->ecs.hasSolidRainbowEffect(entity))
    {
      SolidRainbowEffect& effect = this->ecs.getSolidRainbowEffect(entity);
      if(effect.updateRate > 0 && frames % effect.updateRate == 0)
      {
        effect.apply(text.currentColor, deltaTime);
      }
      else
      {
        text.currentColor = effect.currentColor;
      }
    }
    if(this->ecs.hasSolidRainbowFadeEffect(entity))
    {
      SolidRainbowFadeEffect& effect = this->ecs.getSolidRainbowFadeEffect(entity);
      if(effect.updateRate > 0 && frames % effect.updateRate == 0)
      {
        effect.apply(text.currentColor, deltaTime);
      }
      else
      {
        text.currentColor = effect.currentColor;
      }
    }
    
    for(size_t i = 0; i < text.text.length(); i++)
    {
      vec2<float> charPos = text.penPositions.at(i);

      if(this->ecs.hasParticleEffect(entity))
      {
        ParticleEffect& effect = this->ecs.getParticleEffect(entity);
        effect.apply(charPos, text.position, deltaTime);
      }
      if(this->ecs.hasWaveEffect(entity))
      {
        WaveEffect& effect = this->ecs.getWaveEffect(entity);
        if(effect.updateRate > 0 && frames % effect.updateRate == 0)
        {
          effect.apply(i, charPos, deltaTime);
        }
        else
        {
          charPos += effect.currentPosition.at(i);
        }
      }
      if(this->ecs.hasJitterEffect(entity))
      {
        JitterEffect& effect = this->ecs.getJitterEffect(entity);
        if(effect.updateRate > 0 && frames % effect.updateRate == 0)
        {
          effect.apply(i, charPos, deltaTime);
        }
        else
        {
          charPos += effect.currentPosition.at(i);
        }
      }
      if(this->ecs.hasRainbowEffect(entity))
      {
        RainbowEffect& effect = this->ecs.getRainbowEffect(entity);
        if(effect.updateRate > 0 && frames % effect.updateRate == 0)
        {
          effect.apply(i, text.currentColor, deltaTime);
        }
        else
        {
          text.currentColor = effect.currentColor.at(i);
        }
      }
      if(this->ecs.hasRainbowWaveEffect(entity))
      {
        RainbowWaveEffect& effect = this->ecs.getRainbowWaveEffect(entity);
        if(effect.updateRate > 0 && frames % effect.updateRate == 0)
        {
          effect.apply(i, text.currentColor, deltaTime);
        }
        else
        {
          text.currentColor = effect.currentColor.at(i);
        }
      }
      
      const char& character = text.text.at(i);
      vec2 size = text.atlas->getTileDimensions(std::string{character});
      glr::QuadUVs uvs = text.atlas->getUVsForTile(std::string{character});

      //TODO FIXME this shouldn't need to exist, hb_shape should be giving y offsets but isn't
      if(character == 'p' || character == 'y' || character == 'q' || character == 'j' || character == 'g')
      {
        charPos.y() -= 7;
      }
      
      glr::Renderable r{{text.position.x() + charPos.x(), text.position.y() + charPos.y(), 0.0f}, {size.width(), size.height(), 1}, quat<float>{},
      &*text.texture,
      &textShader,
      &quad,
      1, 0, "text",
      glr::Renderable::CharacterInfo{character, text.currentColor, uvs, "inputColor"}};
      
      out.add({r});
    }

    if(this->ecs.hasParticleEffect(entity))
    {
      out = out + this->ecs.getParticleEffect(entity).makeSceneGraph();
    }
  }
  return out;
}

