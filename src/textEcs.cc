#include "textEcs.hh"

ID TextECS::newEntity(const std::string& text, const std::vector<u8>& font, const std::string& fontName, const u32 pointSize, const glr::Color color, const Language& language, const std::vector<hb_feature_t>& features)
{
  const ID out = this->lastEntity;
  this->lastEntity++;
  this->entities.emplace_back(out);
  this->textBlocks[out] = TextBlock(text, font, fontName, pointSize, color, language, features);
  return out;
}

void TextECS::addJitterEffect(const ID entity)
{
  this->jitterEffects[entity] = JitterEffect(this->textBlocks.at(entity).text.length());
}

void TextECS::addRainbowEffect(const ID entity)
{
  this->rainbowEffects[entity]= RainbowEffect(this->textBlocks.at(entity).text.length());
}

void TextECS::addSolidRainbowEffect(const ID entity)
{
  this->solidRainbowEffects[entity];
}

void TextECS::addSolidRainbowFadeEffect(const ID entity)
{
  this->solidRainbowFadeEffects[entity];
}

TextBlock& TextECS::getText(const ID entity)
{
  return this->textBlocks.at(entity);
}

JitterEffect& TextECS::getJitterEffect(const ID entity)
{
  return this->jitterEffects.at(entity);
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


bool TextECS::hasJitterEffect(const ID entity)
{
  return this->jitterEffects.contains(entity);
}

bool TextECS::hasRainbowEffect(const ID entity)
{
  return this->rainbowEffects.contains(entity);
}

bool TextECS::hasSolidRainbowEffect(const ID entity)
{
  return this->solidRainbowEffects.contains(entity);
}

bool TextECS::hasSolidRainbowFadeEffect(const ID entity)
{
  return this->solidRainbowFadeEffects.contains(entity);
}

