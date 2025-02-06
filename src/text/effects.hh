#pragma once

#include "glrColor.hh"
#include "../util.hh"
#include "../pngFormat.hh"

#include <glrTexture.hh>
#include <glrShader.hh>
#include <glrRenderList.hh>

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

struct WaveEffect
{
  WaveEffect() = default;
  explicit WaveEffect(size_t numGlyphs);
  
  void apply(size_t currentGlyph, vec2<float>& currentPos, float deltaTime);

  float amplitude = 10.0f; //0-100
  float frequency = 20.0f;
  u32 updateRate = 1;
  std::vector<vec2<float>> currentPosition;

  private:
  std::vector<float> currentT{};
};

struct ParticleEffect
{
  struct Particle
  {
    Particle() = default;
    explicit Particle(const vec2<float>& position, const vec2<u32>& size);
    void update(float deltaTime);
    
    vec2<float> position{};
    vec2<float> currentSize{};
    vec2<float> maxSize{};
    quat<float> rotation{};
    
    float timeAlive = 0.0f; //Seconds, how long the particle has existed for
    float lifespan = 0.5f; //Seconds, how long the particle will exist for once created
    float progress = 0.0f; //0-1, %, how close the particle is to reaching the end of its lifespan
  };

  ParticleEffect() = default;
  explicit ParticleEffect(size_t maxParticles);
  
  void apply(const vec2<float>& currentPos, const vec2<float>& offset, float deltaTime);
  void setParticleTexture(const std::string& name, const std::string &path, glr::TexColorFormat colorFmt);
  glr::RenderList makeSceneGraph() const;

  float radius = 50.0f; //How far away from the center of the glyph a particle can spawn
  float spawnThreshold = 0.99f; //0-1, A random number from 0-1 is chosen in apply(), and if the result is >= spawnThreshold, a particle spawns
  vec2<float> particleScale{1, 1};
  float particleLifespan = 0.5f;
  float particleLifespanJitter = 0.1f; //Adjusts lifetime a random amount between -particleLifespanJitter to +particleLifespanJitter
  
  std::vector<std::unique_ptr<Particle>> particles{};
  std::unique_ptr<glr::Texture> particleTexture = nullptr;
  std::unique_ptr<glr::Shader> particleShader = nullptr;
  std::unique_ptr<glr::Mesh> particleMesh = nullptr;

  private:
  u32 maxParticles = 1;
  u32 currentParticles = 0;
  vec2<u32> particleTextureSize{};
};
