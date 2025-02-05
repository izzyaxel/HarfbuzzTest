#pragma once

#include "window.hh"
#include "deltaTime.hh"
#include "text/textRenderer.hh"
#include "text/langs/textFeatures.hh"

struct Application
{
  Application(u32 width, u32 height, u32 targetFPS);
  
  void run();
  
  //Forwarding
  ID newFont(const std::string& filePath) const;
  ID newEntity(const std::string& text, ID font, const std::string& fontName, u32 pointSize, glr::Color color, const Language& language, const std::vector<hb_feature_t>& features) const;
  
  void addJitterEffect(ID entity) const;
  void addWaveEffect(ID entity) const;
  void addRainbowEffect(ID entity) const;
  void addSolidRainbowEffect(ID entity) const;
  void addSolidRainbowFadeEffect(ID entity) const;
  void addRainbowWaveEffect(ID entity) const;
  
  TextBlock& getText(ID entity) const;
  JitterEffect& getJitterEffect(ID entity) const;
  WaveEffect& getWaveEffect(ID entity) const;
  RainbowEffect& getRainbowEffect(ID entity) const;
  SolidRainbowEffect& getSolidRainbowEffect(ID entity) const;
  SolidRainbowFadeEffect& getSolidRainbowFadeEffect(ID entity) const;
  RainbowWaveEffect& getRainbowWaveEffect(ID entity) const;
  
  bool hasJitterEffect(ID entity) const;
  bool hasWaveEffect(ID entity) const;
  bool hasRainbowEffect(ID entity) const;
  bool hasSolidRainbowEffect(ID entity) const;
  bool hasSolidRainbowFadeEffect(ID entity) const;
  bool hasRainbowWaveEffect(ID entity) const;
  
  void addTextToRender(ID text) const;

  std::unique_ptr<TextRenderer> textRenderer = nullptr;
  std::unique_ptr<Window> window = nullptr;
  
  bool exiting = false;
  u32 targetFPS = 0;
  u64 frames = 0;
  DeltaTime deltaTime{[] { return 0.0f; }};
  
};
