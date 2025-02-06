#include "textEcs.hh"

ID TextECS::newEntity(const std::string& text, const ID font, const std::string& fontName, const u32 pointSize, const glr::Color color, const Language& language, const std::vector<hb_feature_t>& features)
{
  if(!this->fontFiles.contains(font))
  {
    return INVALID_ID;
  }
  
  const ID out = this->lastEntity;
  this->lastEntity++;
  this->entities.emplace_back(out);
  this->textBlocks[out] = TextBlock(text, this->fontFiles.at(font), fontName, pointSize, color, language, features);
  return out;
}

ID TextECS::newFontFile(const std::string &filePath)
{
  const ID out = this->lastFontFile;
  this->lastFontFile++;
  this->fontFiles[out] = readFile(filePath);
  return out;
}

void TextECS::addJitterEffect(const ID entity)
{
  this->jitterEffects[entity] = JitterEffect(this->textBlocks.at(entity).text.length());
}

void TextECS::addWaveEffect(const ID entity)
{
  this->waveEffects[entity] = WaveEffect(this->textBlocks.at(entity).text.length());
}

void TextECS::addRainbowEffect(const ID entity)
{
  this->rainbowEffects[entity] = RainbowEffect(this->textBlocks.at(entity).text.length());
}

void TextECS::addSolidRainbowEffect(const ID entity)
{
  this->solidRainbowEffects[entity];
}

void TextECS::addSolidRainbowFadeEffect(const ID entity)
{
  this->solidRainbowFadeEffects[entity];
}

void TextECS::addRainbowWaveEffect(const ID entity)
{
  this->rainbowWaveEffects[entity] = RainbowWaveEffect(this->textBlocks.at(entity).text.length());
}

void TextECS::addParticleEffect(const ID entity, const u32 maxParticles)
{
  this->particleEffects[entity] = ParticleEffect(maxParticles);
}

TextBlock& TextECS::getText(const ID entity)
{
  return this->textBlocks.at(entity);
}

JitterEffect& TextECS::getJitterEffect(const ID entity)
{
  return this->jitterEffects.at(entity);
}

WaveEffect &TextECS::getWaveEffect(const ID entity)
{
  return this->waveEffects.at(entity);
}

RainbowEffect& TextECS::getRainbowEffect(const ID entity)
{
  return this->rainbowEffects.at(entity);
}

SolidRainbowEffect& TextECS::getSolidRainbowEffect(const ID entity)
{
  return this->solidRainbowEffects.at(entity);
}

SolidRainbowFadeEffect& TextECS::getSolidRainbowFadeEffect(const ID entity)
{
  return this->solidRainbowFadeEffects.at(entity);
}

RainbowWaveEffect& TextECS::getRainbowWaveEffect(const ID entity)
{
  return this->rainbowWaveEffects.at(entity);
}

ParticleEffect &TextECS::getParticleEffect(const ID entity)
{
  return this->particleEffects.at(entity);
}

bool TextECS::hasJitterEffect(const ID entity) const
{
  return this->jitterEffects.contains(entity);
}

bool TextECS::hasWaveEffect(const ID entity) const
{
  return this->waveEffects.contains(entity);
}

bool TextECS::hasRainbowEffect(const ID entity) const
{
  return this->rainbowEffects.contains(entity);
}

bool TextECS::hasSolidRainbowEffect(const ID entity) const
{
  return this->solidRainbowEffects.contains(entity);
}

bool TextECS::hasSolidRainbowFadeEffect(const ID entity) const
{
  return this->solidRainbowFadeEffects.contains(entity);
}

bool TextECS::hasRainbowWaveEffect(const ID entity) const
{
  return this->rainbowWaveEffects.contains(entity);
}

bool TextECS::hasParticleEffect(const ID entity) const
{
  return this->particleEffects.contains(entity);
}
