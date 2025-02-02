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

struct RainbowWaveEffect
{
  RainbowWaveEffect() = default;
  explicit RainbowWaveEffect(size_t numGlyphs);
  void apply(size_t currentGlyph, glr::Color& currentColor, float deltaTime);

  float gradientLength = 60; //1-360, how much of the spectrum is visible at once within the block of text, inverse scale
  float saturation = 1.0f; //0-1
  float brightness = 1.0f; //0-1
  u32 updateRate = 1;
  std::vector<float> currentT{};
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
