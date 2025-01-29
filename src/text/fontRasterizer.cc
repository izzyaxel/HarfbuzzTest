#include "fontRasterizer.hh"

ID FontRasterizer::loadFont(const std::string &fontPath, const std::string& name)
{
  if(fontPath.empty())
  {
    return INVALID_ID;
  }
  
  const std::vector<u8> fontData = readFile(fontPath);
  
  if(fontData.empty())
  {
    return INVALID_ID;
  }

  const ID index = this->lastFontFileID;
  this->lastFontFileID++;
  this->fontFiles[index] = std::make_pair(name, fontData);
  return index;
}

ID FontRasterizer::rasterizeFont(const ID font, const u32 pointSize)
{
  if(font == INVALID_ID || !this->fontFiles.contains(font) || pointSize == 0)
  {
    return INVALID_ID;
  }
  
  const ID index = this->lastFontID;
  this->lastFontID++;
  this->fonts[index];

  auto& [name, data] = this->fontFiles[font];
  FontData& fontData = this->fonts.at(index);
  fontData.fontPointSize = pointSize;
  fontData.name = name;
  
  //Start FreeType
  FT_Error ftError = FT_Init_FreeType(&fontData.ftLib);
  if(ftError != 0 || !fontData.ftLib)
  {
    return INVALID_ID;
  }

  //Create FreeType font
  ftError = FT_New_Memory_Face(fontData.ftLib, data.data(), (long)data.size(), 0, &fontData.ftFace);
  if(ftError != 0)
  {
    return INVALID_ID;
  }

  //Set FreeType font size
  ftError = FT_Set_Char_Size(fontData.ftFace, (long)(SCALAR * (float)pointSize), (long)(SCALAR * (float)pointSize), 0, 0);
  if(ftError != 0)
  {
    return INVALID_ID;
  }

  //Create HarfBuzz font from the FreeType face
  fontData.hbFont = hb_ft_font_create_referenced(fontData.ftFace);
  if(!fontData.hbFont)
  {
    return INVALID_ID;
  }

  //Rasterize to the font texture atlas
  for(char i = 33; i < 127; i++) //TODO non-ASCII
  {
    ftError = FT_Load_Char(fontData.ftFace, i, FT_LOAD_RENDER);
    if(ftError != 0)
    {
      continue;
    }
    
    FT_GlyphSlot g = fontData.ftFace->glyph;
    const u32 w = g->bitmap.pitch;
    const u32 h = g->bitmap.rows;
    if(w == 0 || h == 0)
    {
      continue;
    }
    
    fontData.glyphSizes[i] = {w, h};

    //Find the tallest glyph
    if(fontData.glyphSizes.at(i).y() > fontData.tallestGlyph)
    {
      fontData.tallestGlyph = fontData.glyphSizes.at(i).y();
    }
    std::vector next(g->bitmap.buffer, g->bitmap.buffer + w * h);
    if(next.empty())
    {
      continue;
    }
    fontData.atlas.addTile(std::string{i}, glr::TexColorFormat::GREY, std::move(next), w, h);
  }
  
  fontData.atlas.finalize(fontData.name + " " + std::to_string(fontData.fontPointSize) + "pt ID: " + std::to_string(this->lastFontID), fontData.texture, glr::TexColorFormat::GREY);
  fontData.ready = true;
  
  return index;
}

std::vector<vec2<float>> FontRasterizer::shapeText(const std::string& text, const Language& language, const ID fontID)
{
  FontData& fontData = this->fonts.at(fontID);
  
  hb_buffer_t* hbBuffer = hb_buffer_create();
  hb_buffer_set_script(hbBuffer, language.script);
  hb_buffer_set_direction(hbBuffer, language.direction);
  hb_buffer_set_language(hbBuffer, hb_language_from_string(language.lang.c_str(), -1));
  hb_buffer_add_utf8(hbBuffer, text.c_str(), -1, 0, -1);
  hb_shape(fontData.hbFont, hbBuffer, nullptr, 0);
  if(hb_buffer_get_content_type(hbBuffer) != HB_BUFFER_CONTENT_TYPE_GLYPHS)
  {
    return{};
  }

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

vec2<u32> FontRasterizer::getGlyphSize(const ID rasterizedFontID, const char glyph) const
{
  if(!this->fonts.contains(rasterizedFontID))
  {
    return {};
  }
  if(!this->fonts.at(rasterizedFontID).glyphSizes.contains(glyph))
  {
    return {};
  }
  return this->fonts.at(rasterizedFontID).glyphSizes.at(glyph);
}

bool FontRasterizer::isFontRasterized(const ID fontID) const
{
  if(!this->fonts.contains(fontID))
  {
    return false;
  }
  
  return this->fonts.at(fontID).ready;
}

