#pragma once

#include "glrColor.hh"
#include "../util.hh"

struct TextEffect
{
  virtual ~TextEffect() = default;
  virtual void apply(size_t currentGlyph, vec2<float>& currentPos, glr::Color& currentColor) = 0;

  u32 updateRate = 1; //Runs once every x frames
};

/// Each glyph is a random color
struct RainbowEffect : TextEffect
{
  explicit RainbowEffect(size_t numGlyphs);
  void apply(size_t currentGlyph, vec2<float>& currentPos, glr::Color& currentColor) override;
  
  size_t numGlyphs = 1;
  std::vector<vec2<float>> currentColor;
};

/// All glyphs are the same random color
struct SolidRainbowEffect : TextEffect
{
  explicit SolidRainbowEffect(size_t numGlyphs);
  void apply(size_t currentGlyph, vec2<float>& currentPos, glr::Color& currentColor) override;
  
  size_t numGlyphs = 1;
};

/// All glyphs are the same color, and the color does a rainbow fade
struct SolidRainbowFadeEffect : TextEffect
{
  explicit SolidRainbowFadeEffect(size_t numGlyphs);
  void apply(size_t currentGlyph, vec2<float>& currentPos, glr::Color& currentColor) override;

  size_t numGlyphs = 1;
};

/// Each glyph moves around a random amount
struct JitterEffect : TextEffect
{
  explicit JitterEffect(size_t numGlyphs);
  
  void apply(size_t currentGlyph, vec2<float>& currentPos, glr::Color& currentColor) override;

  size_t numGlyphs = 1;
  float amount = 5; //+- pixel value around the center point on both axes
  std::vector<vec2<float>> currentPosition;
};
