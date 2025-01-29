#pragma once

#include "../alias.hh"
#include "fontData.hh"
#include "text.hh"

#include <commons/math/vec2.hh>

struct FontRasterizer
{
  /// Load a font into memory to reuse multiple times
  /// @param fontPath Full path to the font file
  /// @param name The font's name
  /// @return The font file's ID
  ID loadFont(const std::string& fontPath, const std::string& name);
  
  /// Load and rasterize a font
  /// @param fontFileID The font file to use
  /// @param text 
  /// @return The rasterized font's ID
  void rasterizeFont(ID fontFileID, Text& text);

  /// Create a layout with the given text, language, and font
  /// @param text 
  /// @return The pen positions of each glyph in the given text
  void shapeText(Text& text);

  FontData& getFontData(ID fontID);

  /// Get the width/height of a glyph for a given font/point size
  /// @param rasterizedFontID The rasterized font
  /// @param glyph The glyph
  /// @return The size of the glyph
  vec2<u32> getGlyphSize(ID rasterizedFontID, char glyph) const;

  /// Check if a font has been rasterized and uploaded to the GPU
  /// @param fontID The font
  /// @return Yes/no
  bool isFontRasterized(ID fontID) const;
  
  private:
  ID lastFontFileID = 0;
  std::unordered_map<ID, std::pair<std::string, std::vector<u8>>> fontFiles;
  
  ID lastFontID = 0;
  std::unordered_map<ID, FontData> fonts{};
};
