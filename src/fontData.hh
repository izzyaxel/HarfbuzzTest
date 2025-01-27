#pragma once

#include "alias.hh"
#include "util.hh"

#include <harfbuzz/hb-ft.h>
#include <unordered_map>

struct FontData
{
  ~FontData()
  {
    hb_font_destroy(this->hbFont);
    FT_Done_FreeType(this->ftLib);
  }
  
  FT_Library ftLib = nullptr;
  FT_Face ftFace = nullptr;
  hb_font_t* hbFont = nullptr;
  u32 fontPointSize = 0;
  u32 tallestGlyph = 0;
  std::unordered_map<char, vec2<u32>> glyphSizes{};
  bool ready = false;
};
