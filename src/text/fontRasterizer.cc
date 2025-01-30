#include "fontRasterizer.hh"

#include "glrExternal.hh"

ID FontRasterizer::loadFont(const std::string &fontPath, const std::string& name)
{
  if(fontPath.empty())
  {
    printf("loadFont: Font path was empty\n");
    return INVALID_ID;
  }
  
  const std::vector<u8> fontData = readFile(fontPath);
  if(fontData.empty())
  {
    printf("loadFont: Font file was empty\n");
    return INVALID_ID;
  }

  const ID index = this->lastFontFileID;
  this->lastFontFileID++;
  this->fontFiles[index] = std::make_pair(name, fontData);
  return index;
}

void FontRasterizer::rasterizeFont(const ID fontFileID, Text& text)
{
  if(fontFileID == INVALID_ID || !this->fontFiles.contains(fontFileID) || text.pointSize == 0)
  {
    printf("rasterizeFont: Invalid configuration\n");
    return;
  }
  
  const ID index = this->lastFontID;
  this->lastFontID++;
  this->fonts[index];

  auto& [name, data] = this->fontFiles[fontFileID];
  FontData& fontData = this->fonts.at(index);
  fontData.fontPointSize = text.pointSize;
  fontData.name = name;
  
  //Start FreeType
  FT_Error ftError = FT_Init_FreeType(&fontData.ftLib);
  if(ftError != 0 || !fontData.ftLib)
  {
    printf("rasterizeFont: Freetype failed to initialize\n");
    return;
  }

  //Create FreeType font
  ftError = FT_New_Memory_Face(fontData.ftLib, data.data(), (long)data.size(), 0, &fontData.ftFace);
  if(ftError != 0)
  {
    printf("rasterizeFont: Freetype failed to load font\n");
    return;
  }

  //Set FreeType font size
  ftError = FT_Set_Char_Size(fontData.ftFace, (long)(SCALAR * (float)text.pointSize), (long)(SCALAR * (float)text.pointSize), 0, 0);
  if(ftError != 0)
  {
    printf("rasterizeFont: Freetype failed to set font size\n");
    return;
  }

  //Create HarfBuzz font from the FreeType face
  fontData.hbFont = hb_ft_font_create_referenced(fontData.ftFace);
  if(!fontData.hbFont)
  {
    printf("rasterizeFont: Harfbuzz failed to create a font from the Freetype font\n");
    return;
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

  glr::pixelStoreiPack(1);
  glr::pixelStoreiUnpack(1);
  fontData.atlas.finalize(fontData.name + " " + std::to_string(fontData.fontPointSize) + "pt ID: " + std::to_string(this->lastFontID), fontData.texture, glr::TexColorFormat::GREY);
  fontData.ready = true;
  text.fontID = index;
}

void FontRasterizer::shapeText(Text& text)
{
  if(!this->fonts.contains(text.fontID))
  {
    printf("shapeText: Font not found\n");
    return;
  }

  if(text.language.lang.empty() || text.language.script == HB_SCRIPT_UNKNOWN)
  {
    printf("shapeText: Language or script were not set\n");
    return;
  }
  
  const FontData& fontData = this->fonts.at(text.fontID);
  
  hb_buffer_t* hbBuffer = hb_buffer_create();
  hb_buffer_set_script(hbBuffer, text.language.script);
  hb_buffer_set_direction(hbBuffer, text.language.direction);
  hb_buffer_set_language(hbBuffer, hb_language_from_string(text.language.lang.c_str(), -1));
  hb_buffer_add_utf8(hbBuffer, text.text.c_str(), -1, 0, -1);
  
  hb_shape(fontData.hbFont, hbBuffer, text.features.data(), text.features.size());
  if(hb_buffer_get_content_type(hbBuffer) != HB_BUFFER_CONTENT_TYPE_GLYPHS)
  {
    printf("shapeText: Failed to shape text: %s, %upt, \"%s\"\n", fontData.name.c_str(), text.pointSize, text.text.c_str());
    return;
  }

  //Calculate pen positions for each glyph
  u32 bufferLength;
  const hb_glyph_position_t* glyphPos = hb_buffer_get_glyph_positions(hbBuffer, &bufferLength);
  hb_buffer_destroy(hbBuffer);
  double xadv = 0.0, yadv = 0.0;
  for(size_t i = 0; i < bufferLength; i++)
  {
    if(text.text.at(i) == '\n')
    {
      yadv -= (double)fontData.tallestGlyph * SCALAR + (double)LINE_SPACING * SCALAR;
      xadv = 0;
      const auto& gp = glyphPos[i];
      text.penPositions.emplace_back(xadv / SCALAR + gp.x_offset, yadv / SCALAR + gp.y_offset);
      continue;
    }
    const auto& gp = glyphPos[i];
    text.penPositions.emplace_back(xadv / SCALAR + gp.x_offset, yadv / SCALAR + gp.y_offset);
    xadv += gp.x_advance;
    yadv += gp.y_advance;
  }
}

FontData& FontRasterizer::getFontData(const ID fontID)
{
  return this->fonts.at(fontID);
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

