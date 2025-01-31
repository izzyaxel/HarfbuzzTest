#include "effects.hh"

#include "../util.hh"

RainbowEffect::RainbowEffect(const size_t numGlyphs)
{
  this->numGlyphs = numGlyphs;
  this->currentColor.resize(numGlyphs);
}

void RainbowEffect::apply(size_t currentGlyph, glr::Color &currentColor)
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

void SolidRainbowEffect::apply(size_t currentGlyph, glr::Color &currentColor)
{
  
}


SolidRainbowFadeEffect::SolidRainbowFadeEffect(const size_t numGlyphs)
{
  this->numGlyphs = numGlyphs;
}

void SolidRainbowFadeEffect::apply(size_t currentGlyph, glr::Color &currentColor)
{
  
}


JitterEffect::JitterEffect(const size_t numGlyphs)
{
  this->numGlyphs = numGlyphs;
  this->currentPosition.resize(numGlyphs);
  this->effectType = Effect::JITTER;
}

void JitterEffect::apply(const size_t currentGlyph, vec2<float> &currentPos)
{
  if(this->currentPosition.size() < currentGlyph)
  {
    return;
  }
  this->currentPosition.at(currentGlyph) = {randomFloat(-this->amount, this->amount), randomFloat(-this->amount, this->amount)};
  currentPos += this->currentPosition.at(currentGlyph);
}
