#include "textBlock.hh"
#include "../util.hh"

#include <harfbuzz/hb-ft.h>
#include <harfbuzz/hb.h>
#include <glrExternal.hh>

TextBlock::TextBlock(const std::string& text, const std::vector<u8>& font, const std::string& fontName, const u32 pointSize, const glr::Color color, const Language& language, const std::vector<hb_feature_t>& features)
{
  if(font.empty() || pointSize == 0 || text.empty() || language.lang.empty() || language.script == HB_SCRIPT_UNKNOWN)
  {
    return;
  }
  
  this->text = text;
  this->name = fontName;
  this->pointSize = pointSize;
  this->language = language;
  this->features = features;
  this->currentColor = color;
  
  this->texture = std::make_unique<glr::Texture>();
  this->atlas = std::make_unique<glr::Atlas>();

  //Rasterize
  //Start FreeType
  FT_Library ftLib = nullptr;
  FT_Face ftFace = nullptr;
  hb_font_t* hbFont = nullptr;
  FT_Error ftError = FT_Init_FreeType(&ftLib);
  if(ftError != 0 || !ftLib)
  {
    printf("rasterizeFont: Freetype failed to initialize\n");
    return;
  }

  //Create FreeType font
  ftError = FT_New_Memory_Face(ftLib, font.data(), (long)font.size(), 0, &ftFace);
  if(ftError != 0)
  {
    printf("rasterizeFont: Freetype failed to load font\n");
    return;
  }

  //Set FreeType font size
  ftError = FT_Set_Char_Size(ftFace, (long)(SCALAR * (float)this->pointSize), (long)(SCALAR * (float)this->pointSize), 0, 0);
  if(ftError != 0)
  {
    printf("rasterizeFont: Freetype failed to set font size\n");
    return;
  }

  //Create HarfBuzz font from the FreeType face
  hbFont = hb_ft_font_create_referenced(ftFace);
  if(!hbFont)
  {
    printf("rasterizeFont: Harfbuzz failed to create a font from the Freetype font\n");
    return;
  }

  //Rasterize to the font texture atlas
  for(char i = 33; i < 127; i++) //TODO non-ASCII
  {
    ftError = FT_Load_Char(ftFace, i, FT_LOAD_RENDER);
    if(ftError != 0)
    {
      continue;
    }
    
    FT_GlyphSlot g = ftFace->glyph;
    const u32 w = g->bitmap.pitch;
    const u32 h = g->bitmap.rows;
    if(w == 0 || h == 0)
    {
      continue;
    }
    
    this->glyphSizes[i] = {w, h};

    //Find the tallest glyph
    if(this->glyphSizes.at(i).y() > this->tallestGlyph)
    {
      this->tallestGlyph = this->glyphSizes.at(i).y();
    }
    std::vector next(g->bitmap.buffer, g->bitmap.buffer + w * h);
    if(next.empty())
    {
      continue;
    }
    this->atlas->addTile(std::string{i}, glr::TexColorFormat::GREY, std::move(next), w, h);
  }

  glr::pixelStoreiPack(1);
  glr::pixelStoreiUnpack(1);
  this->atlas->finalize(this->name + " " + std::to_string(this->pointSize) + "pt", *this->texture, glr::TexColorFormat::GREY);
  
  //Shape text
  hb_buffer_t* hbBuffer = hb_buffer_create();
  hb_buffer_set_script(hbBuffer, this->language.script);
  hb_buffer_set_direction(hbBuffer, this->language.direction);
  hb_buffer_set_language(hbBuffer, hb_language_from_string(this->language.lang.c_str(), -1));
  hb_buffer_add_utf8(hbBuffer, this->text.c_str(), -1, 0, -1);
  
  hb_shape(hbFont, hbBuffer, this->features.data(), this->features.size());
  if(hb_buffer_get_content_type(hbBuffer) != HB_BUFFER_CONTENT_TYPE_GLYPHS)
  {
    printf("shapeText: Failed to shape text: %s, %upt, \"%s\"\n", this->name.c_str(), this->pointSize, text.c_str());
    return;
  }

  //Calculate pen positions for each glyph
  u32 bufferLength;
  const hb_glyph_position_t* glyphPos = hb_buffer_get_glyph_positions(hbBuffer, &bufferLength);
  double xadv = 0.0, yadv = 0.0;
  for(size_t i = 0; i < bufferLength; i++)
  {
    const auto& gp = glyphPos[i];
    if(text.at(i) == '\n')
    {
      yadv -= (double)this->tallestGlyph * SCALAR + (double)this->lineSpacing * SCALAR;
      xadv = 0;
      this->penPositions.emplace_back(xadv / SCALAR + gp.x_offset, yadv / SCALAR + gp.y_offset);
      continue;
    }
    this->penPositions.emplace_back(xadv / SCALAR + gp.x_offset, yadv / SCALAR + gp.y_offset);
    xadv += gp.x_advance;
    yadv += gp.y_advance;
  }

  hb_buffer_destroy(hbBuffer);
  hb_font_destroy(hbFont);
  FT_Done_Face(ftFace);
  FT_Done_FreeType(ftLib);
}

vec2<u32> TextBlock::getGlyphSize(const char glyph) const
{
  if(!this->glyphSizes.contains(glyph))
  {
    return {};
  }
  return this->glyphSizes.at(glyph);
}
