#include "effects.hh"
#include "../util.hh"
#include "../shaders.hh"

#include <numbers>
#include <commons/misc.hh>

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
  this->t += 0.1f * deltaTime;
  if(this->t > 1.0f)
  {
    this->t = 0.0f;
  }
}

RainbowWaveEffect::RainbowWaveEffect(const size_t numGlyphs)
{
  this->currentColor.resize(numGlyphs);
  this->currentT.resize(numGlyphs);
  float cur = 0.0f;
  const float norm = this->gradientLength / 360.0f;
  const float stepSize = norm / (float)numGlyphs;
  
  for(float& t : this->currentT)
  {
    t = cur;
    cur += stepSize;
  }
}

void RainbowWaveEffect::apply(const size_t currentGlyph, glr::Color& currentColor, const float deltaTime)
{
  float& t = this->currentT.at(currentGlyph);
  float hue = lerp(0.0f, 360.0f, t);
  vec3<float> rgb = hsvTorgb({hue, this->saturation, this->brightness});
  currentColor.fromRGBAf(rgb.r(), rgb.g(), rgb.b(), 1);
  this->currentColor.at(currentGlyph) = currentColor;
  
  t += 0.1f * deltaTime;
  if(t > 1.0f)
  {
    t = 0.0f;
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

WaveEffect::WaveEffect(const size_t numGlyphs)
{
  this->currentT.resize(numGlyphs);
  this->currentPosition.resize(numGlyphs);
  const float norm = this->amplitude / 100.0f;
  const float stepSize = norm / (float)numGlyphs;
  float cur = 0.0f;
  for(float& t : this->currentT)
  {
    t = cur;
    cur += stepSize;
  }
}

void WaveEffect::apply(const size_t currentGlyph, vec2<float> &currentPos, const float deltaTime)
{
  if(this->currentPosition.size() < currentGlyph)
  {
    return;
  }
  
  float& t = this->currentT.at(currentGlyph);
  currentPos.y() += this->amplitude * std::sin(2.0 * std::numbers::pi * this->frequency * t);
  this->currentPosition.at(currentGlyph) = currentPos;
  
  t += 0.1f * deltaTime;
  if(t > 1.0f)
  {
    t = 0.0f;
  }
}

ParticleEffect::Particle::Particle(const vec2<float>& position, const vec2<u32>& size)
{
  this->position = position;
  this->maxSize = vec2<float>{size};
  this->currentSize = this->maxSize;
  this->lifespan += randomFloat(-0.1f, 0.1f);
}

void ParticleEffect::Particle::update(const float deltaTime)
{
  this->timeAlive += deltaTime * deltaTime;
  this->progress = bound(this->timeAlive / this->lifespan, 0.0f, 1.0f);
  this->currentSize.x() = lerp(this->maxSize.x(), 0.0f, this->progress);
  this->currentSize.y() = lerp(this->maxSize.y(), 0.0f, this->progress);
}

ParticleEffect::ParticleEffect(const size_t maxParticles)
{
  constexpr std::array<float, 8>  orthoQuadUVs{1, 0, 0, 0, 1, 1, 0, 1};
  constexpr std::array<float, 12> cQuadVerts{0.5f, 0.5f, 0, -0.5f, 0.5f, 0, 0.5f, -0.5f, 0, -0.5f, -0.5f, 0};
  this->particleMesh = std::make_unique<glr::Mesh>(cQuadVerts, orthoQuadUVs);
  this->particleShader = std::make_unique<glr::Shader>("Object shader", vertSrc, fragSrc);
  
  this->maxParticles = maxParticles;
  this->particles.resize(maxParticles);
}

void ParticleEffect::setParticleTexture(const std::string& name, const std::string &path, const glr::TexColorFormat colorFmt)
{
  PNG png = decodePNG(path);
  this->particleTextureSize.width() = png.width;
  this->particleTextureSize.height() = png.height;
  this->particleTexture = std::make_unique<glr::Texture>(name, png.imageData.data(), this->particleTextureSize.width(), this->particleTextureSize.height(), colorFmt);
}

void ParticleEffect::apply(const vec2<float> &currentPos, const vec2<float>& offset, const float deltaTime)
{
  if(this->currentParticles < this->maxParticles)
  {
    const float rng = randomFloat(0.0f, 1.0f);
    if(rng >= this->spawnThreshold)
    {
      for(auto& particle : this->particles)
      {
        if(!particle)
        {
          //TODO random position in circular bounds?
          const float x = randomFloat(-this->radius, this->radius);
          const float y = randomFloat(-this->radius, this->radius);
          particle = std::make_unique<Particle>(currentPos + offset + vec2{x, y}, this->particleTextureSize);
          this->currentParticles++;
          break;
        }
      }
    }
  }
  
  for(auto& particle : this->particles)
  {
    if(!particle)
    {
      continue;
    }
    
    particle->update(deltaTime);
    if(particle->timeAlive >= particle->lifespan)
    {
      particle = nullptr;
      this->currentParticles--;
    }
  }
}

glr::RenderList ParticleEffect::makeSceneGraph() const
{
  glr::RenderList out;
  for(const auto& particle : this->particles)
  {
    if(!particle)
    {
      continue;
    }
    
    glr::Renderable r{vec3{particle->position, 0.0f}, vec3{particle->currentSize * this->particleScale, 1.0f}, particle->rotation, this->particleTexture.get(), this->particleShader.get(), this->particleMesh.get(), 0, 0, "particle"};
    out.add({r});
  }
  return out;
}
