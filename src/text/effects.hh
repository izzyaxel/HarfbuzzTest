#pragma once

#include "glrColor.hh"
#include "../util.hh"

/// Each glyph is a random color
struct RainbowEffect
{
  RainbowEffect() = default;
  explicit RainbowEffect(size_t numGlyphs);
  void apply(size_t currentGlyph, glr::Color& currentColor, float deltaTime);

  u32 updateRate = 1; //Runs once every x frames
  std::vector<glr::Color> currentColor;
};

/// All glyphs are the same random color
struct SolidRainbowEffect
{
  void apply(glr::Color& currentColor, float deltaTime);

  u32 updateRate = 1;
  glr::Color currentColor;
};

/// All glyphs are the same color, and the color does a rainbow fade
struct SolidRainbowFadeEffect
{
  void apply(glr::Color& currentColor, float deltaTime);

  u32 updateRate = 1;
  glr::Color currentColor;
  float t = 0.0f;
};

/// Each glyph moves around a random amount
struct JitterEffect
{
  JitterEffect() = default;
  explicit JitterEffect(size_t numGlyphs);
  
  void apply(size_t currentGlyph, vec2<float>& currentPos, float deltaTime);

  u32 updateRate = 1;
  float amount = 5; //+- pixel value around the center point on both axes
  std::vector<vec2<float>> currentPosition;
};
