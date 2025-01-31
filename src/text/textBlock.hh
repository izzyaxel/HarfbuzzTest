#pragma once

#include "../alias.hh"
#include "effects.hh"
#include "langs/textLanguage.hh"

#include <commons/math/vec2.hh>
#include <glrColor.hh>
#include <glrAtlas.hh>
#include <unordered_map>

#include "justification.hh"

struct TextBlock
{
  TextBlock(const std::string& text, const std::vector<u8>& font, const std::string& fontName, u32 pointSize, glr::Color color, const Language& language, const std::vector<hb_feature_t>& features);
  
  vec2<u32> getGlyphSize(char glyph) const;

  std::unique_ptr<glr::Atlas> atlas;
  std::unique_ptr<glr::Texture> texture;
  
  u32 tallestGlyph = 0;
  std::unordered_map<char, vec2<u32>> glyphSizes{};
  std::vector<vec2<float>> penPositions{};
  
  std::string text;
  std::string name;
  vec2<float> pos;
  u32 lineSpacing = 10;
  u32 pointSize = 0;
  Language language{};
  std::vector<hb_feature_t> features{};
  glr::Color color{};
  std::vector<TextEffect*> effects{};
  Justification justification = Justification::LEFT;
};
