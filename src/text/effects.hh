#pragma once

#include "glrColor.hh"
#include "../util.hh"

enum struct Effect
{
  RAINBOW,
  SOLID_RAINBOW,
  SOLID_RAINBOW_FADE,
  JITTER,
};

struct TextEffect
{
  virtual ~TextEffect() = default;

  Effect effectType;
};

/// Each glyph is a random color
struct RainbowEffect : TextEffect
{
  explicit RainbowEffect(size_t numGlyphs);
  void apply(size_t currentGlyph, glr::Color& currentColor);
  
  u32 updateRate = 1; //Runs once every x frames
  size_t numGlyphs = 1;
  std::vector<vec2<float>> currentColor;
};

/// All glyphs are the same random color
struct SolidRainbowEffect : TextEffect
{
  explicit SolidRainbowEffect(size_t numGlyphs);
  void apply(size_t currentGlyph, glr::Color& currentColor);
  
  u32 updateRate = 1;
  size_t numGlyphs = 1;
};

/// All glyphs are the same color, and the color does a rainbow fade
struct SolidRainbowFadeEffect : TextEffect
{
  explicit SolidRainbowFadeEffect(size_t numGlyphs);
  void apply(size_t currentGlyph, glr::Color& currentColor);

  size_t numGlyphs = 1;
  u32 updateRate = 1;
};

/// Each glyph moves around a random amount
struct JitterEffect : TextEffect
{
  explicit JitterEffect(size_t numGlyphs);
  
  void apply(size_t currentGlyph, vec2<float>& currentPos);

  size_t numGlyphs = 1;
  u32 updateRate = 1;
  float amount = 5; //+- pixel value around the center point on both axes
  std::vector<vec2<float>> currentPosition;
};
