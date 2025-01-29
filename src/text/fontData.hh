#pragma once

#include "../util.hh"

#include <glrAtlas.hh>
#include <harfbuzz/hb-ft.h>
#include <unordered_map>

struct FontData
{
  ~FontData()
  {
    if(this->hbFont)
    {
      hb_font_destroy(this->hbFont);
    }
    if(this->ftFace)
    {
      FT_Done_Face(this->ftFace);
    }
    if(this->ftLib)
    {
      FT_Done_FreeType(this->ftLib);
    }
  }
  
  glr::Atlas atlas;
  glr::Texture texture;
  FT_Library ftLib = nullptr;
  FT_Face ftFace = nullptr;
  hb_font_t* hbFont = nullptr;
  u32 fontPointSize = 0;
  u32 tallestGlyph = 0;
  std::unordered_map<char, vec2<u32>> glyphSizes{};
  bool ready = false;
  std::string name;
};
