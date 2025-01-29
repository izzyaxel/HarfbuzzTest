#pragma once
#include "../util.hh"
#include "langs/textFeatures.hh"
#include "langs/textLanguage.hh"

struct Text
{
  explicit Text(const u32 pointSize, std::string text, Language language) : pointSize(pointSize), text(std::move(text)), language(std::move(language)) {}
  
  ID fontID = INVALID_ID;
  u32 pointSize = 0;
  std::string text;
  Language language{};
  std::vector<hb_feature_t> features{};
  std::vector<vec2<float>> penPositions{};
};
