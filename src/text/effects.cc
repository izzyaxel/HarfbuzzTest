#include "effects.hh"

#include "../util.hh"

RainbowEffect::RainbowEffect(const size_t numGlyphs)
{
  this->currentColor.resize(numGlyphs);
}

void RainbowEffect::apply(const size_t currentGlyph, glr::Color& currentColor, const float deltaTime)
{
  if(this->currentColor.size() < currentGlyph)
  {
    return;
  }
  this->currentColor.at(currentGlyph).fromRGBAf(randomFloat(0, 1), randomFloat(0, 1), randomFloat(0, 1), 1);
  currentColor = this->currentColor.at(currentGlyph);
}

void SolidRainbowEffect::apply(glr::Color& currentColor, const float deltaTime)
{
  this->currentColor.fromRGBAf(randomFloat(0, 1), randomFloat(0, 1), randomFloat(0, 1), 1);
  currentColor = this->currentColor;
}

void SolidRainbowFadeEffect::apply(glr::Color& currentColor, const float deltaTime)
{
  float hue = lerp(0.0f, 360.0f, this->t);
  vec3<float> rgb = hsvTorgb({hue, 1.0f, 1.0f});
  currentColor.fromRGBAf(rgb.r(), rgb.g(), rgb.b(), 1);
  this->t += 0.0001f;
  if(this->t > 1.0f)
  {
    this->t = 0.0f;
  }
}

JitterEffect::JitterEffect(const size_t numGlyphs)
{
  this->currentPosition.resize(numGlyphs);
}

void JitterEffect::apply(const size_t currentGlyph, vec2<float>& currentPos, const float deltaTime)
{
  if(this->currentPosition.size() < currentGlyph)
  {
    return;
  }
  this->currentPosition.at(currentGlyph) = {randomFloat(-this->amount, this->amount), randomFloat(-this->amount, this->amount)};
  currentPos += this->currentPosition.at(currentGlyph);
}
