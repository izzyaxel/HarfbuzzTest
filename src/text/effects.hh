#pragma once

#include "glrColor.hh"
#include "../util.hh"

/// Each glyph is a random color that changes over time
struct RainbowEffect
{
  RainbowEffect() = default;
  explicit RainbowEffect(size_t numGlyphs);
  void apply(size_t currentGlyph, glr::Color& currentColor, float deltaTime);

  u32 updateRate = 1; //Runs once every x frames
  std::vector<glr::Color> currentColor;
};

/// All glyphs are the same random color that changes over time
struct SolidRainbowEffect
{
  void apply(glr::Color& currentColor, float deltaTime);

  u32 updateRate = 1;
  glr::Color currentColor;
};

/// All glyphs are the same color, and the color does a rainbow fade over time
struct SolidRainbowFadeEffect
{
  void apply(glr::Color& currentColor, float deltaTime);

  u32 updateRate = 1;
  glr::Color currentColor;

  private:
  float t = 0.0f;
};

/// Each glyph is a color adjacent to the previous one, and the colors do a rainbow fade over time
struct RainbowWaveEffect
{
  RainbowWaveEffect() = default;
  explicit RainbowWaveEffect(size_t numGlyphs);
  void apply(size_t currentGlyph, glr::Color& currentColor, float deltaTime);

  float gradientLength = 360; //0-360, degrees, how much of the spectrum is visible at once
  float saturation = 1.0f; //0-1, %
  float brightness = 1.0f; //0-1, %
  u32 updateRate = 1;
  std::vector<glr::Color> currentColor{};

  private:
  std::vector<float> currentT{};
};

/// Each glyph moves around a random amount over time
struct JitterEffect
{
  JitterEffect() = default;
  explicit JitterEffect(size_t numGlyphs);
  
  void apply(size_t currentGlyph, vec2<float>& currentPos, float deltaTime);

  u32 updateRate = 1;
  float amount = 5; //+- pixel value around the center point on both axes
  
  std::vector<vec2<float>> currentPosition;
};
