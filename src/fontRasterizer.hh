#pragma once

#include "alias.hh"
#include "fontData.hh"
#include "langs/textLanguage.hh"

struct FontRasterizer
{
  /// Load and prime a font for rasterization
  /// @param fontPath A full path to a font file
  /// @param name The font's name
  /// @param pointSize The point size the font should be rasterized at, each size requires a separate texture atlas
  /// @return The new font's ID
  ID addFont(const std::string& fontPath, const std::string& name, u32 pointSize);
  
  /// Load and prime a font for rasterization
  /// @param fontData The font file as an array of bytes
  /// @param name The font's name
  /// @param pointSize The point size the font should be rasterized at, each size requires a separate texture atlas
  /// @return The new font's ID
  ID addFont(const std::vector<u8>& fontData, const std::string& name, u32 pointSize);

  /// Pack and rasterize the given font to a texture atlas
  /// @param font The font's ID
  /// @return The OpenGL atlas the font was rasterized to
  ID rasterizeFont(ID font);

  /// Create a layout with the given text, language, and font
  /// @param text The text
  /// @param language The language
  /// @param fontID The font's ID
  /// @return The pen positions of each glyph in the given text
  std::vector<vec2<float>> shapeText(const std::string& text, const Language& language, ID fontID);

  FontData& getFont(ID fontID);

  vec2<u32> getGlyphSize(ID fontID, char glyph);

  bool isFontReady(ID fontID);
  
  private:
  ID lastFontID = 0;
  std::unordered_map<ID, FontData> fonts{};
  FontData nullFont{};
};
