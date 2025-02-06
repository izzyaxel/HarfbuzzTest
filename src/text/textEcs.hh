#pragma once

#include "../util.hh"
#include "textBlock.hh"

struct TextECS
{
  ID newEntity(const std::string& text, ID font, const std::string& fontName, u32 pointSize, glr::Color color, const Language& language, const std::vector<hb_feature_t>& features);

  ID newFontFile(const std::string& filePath);
  
  void addJitterEffect(ID entity);
  void addWaveEffect(ID entity);
  void addRainbowEffect(ID entity);
  void addSolidRainbowEffect(ID entity);
  void addSolidRainbowFadeEffect(ID entity);
  void addRainbowWaveEffect(ID entity);
  void addParticleEffect(ID entity, u32 maxParticles);
  
  TextBlock& getText(ID entity);
  JitterEffect& getJitterEffect(ID entity);
  WaveEffect& getWaveEffect(ID entity);
  RainbowEffect& getRainbowEffect(ID entity);
  SolidRainbowEffect& getSolidRainbowEffect(ID entity);
  SolidRainbowFadeEffect& getSolidRainbowFadeEffect(ID entity);
  RainbowWaveEffect& getRainbowWaveEffect(ID entity);
  ParticleEffect& getParticleEffect(ID entity);

  bool hasJitterEffect(ID entity) const;
  bool hasWaveEffect(ID entity) const;
  bool hasRainbowEffect(ID entity) const;
  bool hasSolidRainbowEffect(ID entity) const;
  bool hasSolidRainbowFadeEffect(ID entity) const;
  bool hasRainbowWaveEffect(ID entity) const;
  bool hasParticleEffect(ID entity) const;

  private:
  std::vector<ID> entities{};
  std::unordered_map<ID, std::vector<u8>> fontFiles{};
  std::unordered_map<ID, TextBlock> textBlocks{};
  std::unordered_map<ID, JitterEffect> jitterEffects{};
  std::unordered_map<ID, WaveEffect> waveEffects{};
  std::unordered_map<ID, RainbowEffect> rainbowEffects{};
  std::unordered_map<ID, SolidRainbowEffect> solidRainbowEffects{};
  std::unordered_map<ID, SolidRainbowFadeEffect> solidRainbowFadeEffects{};
  std::unordered_map<ID, RainbowWaveEffect> rainbowWaveEffects{};
  std::unordered_map<ID, ParticleEffect> particleEffects{};

  ID lastEntity = 0;
  ID lastFontFile = 0;
};
