#include "fontRasterizer.hh"

size_t FontRasterizer::addFont(const std::vector<u8>& fontData, const u32 pointSize)
{
  const size_t index = this->last;
  this->last++;
  this->fonts[index] = {};
  
  //Start FreeType
  FT_Error ftError = FT_Init_FreeType(&this->fonts.at(index).ftLib);
  if(ftError != 0 || !this->fonts.at(index).ftLib)
  {
    return INVALID_ID;
  }

  //Create FreeType font
  ftError = FT_New_Memory_Face(this->fonts.at(index).ftLib, fontData.data(), (long)fontData.size(), 0, &this->fonts.at(index).ftFace);
  if(ftError != 0)
  {
    return INVALID_ID;
  }

  //Set FreeType font size
  ftError = FT_Set_Char_Size(this->fonts.at(index).ftFace, (long)(SCALAR * (float)pointSize), (long)(SCALAR * (float)pointSize), 0, 0);
  if(ftError != 0)
  {
    return INVALID_ID;
  }

  //Create HarfBuzz font from the FreeType face
  this->fonts.at(index).hbFont = hb_ft_font_create_referenced(this->fonts.at(index).ftFace);
  if(!this->fonts.at(index).hbFont)
  {
    return INVALID_ID;
  }

  this->fonts.at(index).fontPointSize = pointSize;
  return index;
}

void FontRasterizer::shapeText(const std::string& text, const Language& language, const ID fontID) const
{
  hb_buffer_t* hbBuffer = hb_buffer_create();
  hb_buffer_set_script(hbBuffer, language.script);
  hb_buffer_set_direction(hbBuffer, language.direction);
  hb_buffer_set_language(hbBuffer, hb_language_from_string(language.lang.c_str(), -1));
  hb_buffer_add_utf8(hbBuffer, text.c_str(), -1, 0, -1);
  hb_font_t* hbFont = hb_font_reference(this->fonts.at(fontID).hbFont);
  hb_shape(hbFont, hbBuffer, nullptr, 0);
  if(hb_buffer_get_content_type(hbBuffer) != HB_BUFFER_CONTENT_TYPE_GLYPHS)
  {
    hb_font_destroy(hbFont);
    printf("Buffer failed to be shaped\n");
    return;
  }
  hb_font_destroy(hbFont);
  hb_buffer_destroy(hbBuffer);
}
