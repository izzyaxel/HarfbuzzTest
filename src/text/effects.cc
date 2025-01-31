#include "effects.hh"

#include "../util.hh"

RainbowEffect::RainbowEffect(const size_t numGlyphs)
{
  this->numGlyphs = numGlyphs;
  this->currentColor.resize(numGlyphs);
}

void RainbowEffect::apply(const size_t currentGlyph, vec2<float>& currentPos, glr::Color& currentColor)
{
  if(this->currentColor.size() < currentGlyph)
  {
    return;
  }
  
}


SolidRainbowEffect::SolidRainbowEffect(const size_t numGlyphs)
{
  this->numGlyphs = numGlyphs;
}

void SolidRainbowEffect::apply(const size_t currentGlyph, vec2<float>& currentPos, glr::Color& currentColor)
{
  
}


SolidRainbowFadeEffect::SolidRainbowFadeEffect(const size_t numGlyphs)
{
  this->numGlyphs = numGlyphs;
}

void SolidRainbowFadeEffect::apply(const size_t currentGlyph, vec2<float>& currentPos, glr::Color& currentColor)
{
  
}


JitterEffect::JitterEffect(const size_t numGlyphs)
{
  this->numGlyphs = numGlyphs;
  this->currentPosition.resize(numGlyphs);
}

void JitterEffect::apply(const size_t currentGlyph, vec2<float>& currentPos, glr::Color& currentColor)
{
  if(this->currentPosition.size() < currentGlyph)
  {
    return;
  }
  this->currentPosition.at(currentGlyph) = {randomFloat(-this->amount, this->amount), randomFloat(-this->amount, this->amount)};
  currentPos += this->currentPosition.at(currentGlyph);
}
