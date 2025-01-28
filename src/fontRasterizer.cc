#include "fontRasterizer.hh"

ID FontRasterizer::addFont(const std::string& fontPath, const std::string& name, u32 pointSize)
{
  const std::vector<u8> fontData = readFile(fontPath);
  
  if(fontData.empty())
  {
    return INVALID_ID;
  }
  
  return this->addFont(fontData, name, pointSize);
}

ID FontRasterizer::addFont(const std::vector<u8>& fontData, const std::string& name, const u32 pointSize)
{
  if(pointSize == 0)
  {
    return INVALID_ID;
  }
  
  const ID index = this->lastFontID;
  this->lastFontID++;
  this->fonts[index] = {};

  FontData& font = this->fonts.at(index);
  font.fontPointSize = pointSize;
  font.name = name;
  
  //Start FreeType
  FT_Error ftError = FT_Init_FreeType(&font.ftLib);
  if(ftError != 0 || !font.ftLib)
  {
    return INVALID_ID;
  }

  //Create FreeType font
  ftError = FT_New_Memory_Face(font.ftLib, fontData.data(), (long)fontData.size(), 0, &font.ftFace);
  if(ftError != 0)
  {
    return INVALID_ID;
  }

  //Set FreeType font size
  ftError = FT_Set_Char_Size(font.ftFace, (long)(SCALAR * (float)pointSize), (long)(SCALAR * (float)pointSize), 0, 0);
  if(ftError != 0)
  {
    return INVALID_ID;
  }

  //Create HarfBuzz font from the FreeType face
  font.hbFont = hb_ft_font_create_referenced(font.ftFace);
  if(!font.hbFont)
  {
    return INVALID_ID;
  }
  
  return index;
}

ID FontRasterizer::rasterizeFont(const ID font)
{
  if(!this->fonts.contains(font))
  {
    return INVALID_ID;
  }

  FontData& fontData = this->fonts.at(font);
  const ID out = 0;
  //ID out = newAtlas();
  FT_Error ftError = 0;
  
  //Rasterize to the font texture atlas
  for(char i = 33; i < 127; i++) //TODO non-ASCII
  {
    ftError = FT_Load_Char(fontData.ftFace, i, FT_LOAD_RENDER);
    if(ftError != 0)
    {
      continue;
    }
    const FT_GlyphSlot g = fontData.ftFace->glyph;
    const u32 w = g->bitmap.pitch;
    const u32 h = g->bitmap.rows;
    if(w == 0 || h == 0)
    {
      continue;
    }
    fontData.glyphSizes[i] = {w, h};
    if(fontData.glyphSizes[i].y() > fontData.tallestGlyph) //Find the tallest glyph
    {
      fontData.tallestGlyph = fontData.glyphSizes[i].y();
    }
    std::vector next(g->bitmap.buffer, g->bitmap.buffer + w * h);
    if(next.empty())
    {
      continue;
    }
    //getAtlas(out).addTile(std::string{i}, glr::TexColorFormat::GREY, std::move(next), w, h);
  }
  
  //finalizeAtlas(fontData.name + " " + std::to_string(fontData.fontPointSize) + "pt ID: " + std::to_string(this->lastFontID), out, atlasTextureID, fmt);

  fontData.ready = true;
  
  return out;
}

std::vector<vec2<float>> FontRasterizer::shapeText(const std::string& text, const Language& language, const ID fontID)
{
  FontData& fontData = this->fonts.at(fontID);
  
  hb_buffer_t* hbBuffer = hb_buffer_create();
  hb_buffer_set_script(hbBuffer, language.script);
  hb_buffer_set_direction(hbBuffer, language.direction);
  hb_buffer_set_language(hbBuffer, hb_language_from_string(language.lang.c_str(), -1));
  hb_buffer_add_utf8(hbBuffer, text.c_str(), -1, 0, -1);
  hb_font_t* hbFont = hb_font_reference(fontData.hbFont);
  hb_shape(hbFont, hbBuffer, nullptr, 0);
  if(hb_buffer_get_content_type(hbBuffer) != HB_BUFFER_CONTENT_TYPE_GLYPHS)
  {
    hb_font_destroy(hbFont);
    return{};
  }
  hb_font_destroy(hbFont);

  //Calculate pen positions for each glyph
  u32 bufferLength;
  const hb_glyph_position_t* glyphPos = hb_buffer_get_glyph_positions(hbBuffer, &bufferLength);
  
  hb_buffer_destroy(hbBuffer);
  
  std::vector<vec2<float>> penPositions{};
  double xadv = 0.0, yadv = 0.0;
  for(size_t i = 0; i < bufferLength; i++)
  {
    if(text.at(i) == '\n')
    {
      yadv -= (double)fontData.tallestGlyph * SCALAR + (double)LINE_SPACING * SCALAR;
      xadv = 0;
      const auto& gp = glyphPos[i];
      penPositions.emplace_back(xadv / SCALAR + gp.x_offset, yadv / SCALAR + gp.y_offset);
      continue;
    }
    const auto& gp = glyphPos[i];
    penPositions.emplace_back(xadv / SCALAR + gp.x_offset, yadv / SCALAR + gp.y_offset);
    xadv += gp.x_advance;
    yadv += gp.y_advance;
  }
  return penPositions;
}

FontData& FontRasterizer::getFont(const ID fontID)
{
  return this->fonts.contains(fontID) ? this->fonts.at(fontID) : this->nullFont;
}

vec2<u32> FontRasterizer::getGlyphSize(const ID fontID, const char glyph)
{
  if(!this->fonts.contains(fontID))
  {
    return {};
  }
  if(!this->fonts.at(fontID).glyphSizes.contains(glyph))
  {
    return {};
  }
  return this->fonts.at(fontID).glyphSizes.at(glyph);
}

bool FontRasterizer::isFontReady(ID fontID)
{
  if(!this->fonts.contains(fontID))
  {
    return false;
  }
  
  return this->fonts.at(fontID).ready;
}
