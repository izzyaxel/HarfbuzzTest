#pragma once

#include "alias.hh"
#include "fontData.hh"
#include "langs/textLanguage.hh"

#include <vector>

struct FontRasterizer
{
  size_t addFont(const std::vector<u8>& fontData, u32 pointSize);
  void shapeText(const std::string& text, const Language& language, ID fontID) const;
  
  private:
  ID last = 0;
  std::unordered_map<ID, FontData> fonts{};
};
