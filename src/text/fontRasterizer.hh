#pragma once

#include "../alias.hh"
#include "fontData.hh"
#include "langs/textLanguage.hh"

#include <commons/math/vec2.hh>

struct FontRasterizer
{
  /// Load a font into memory to reuse multiple times
  /// @param fontPath Full path to the font file
  /// @param name The font's name
  /// @return The font file's ID
  ID loadFont(const std::string& fontPath, const std::string& name);
  
  /// Load and rasterize a font
  /// @param font The font file to use
  /// @param pointSize The point size the font should be rasterized at, each size requires a separate texture atlas
  /// @return The rasterized font's ID
  ID rasterizeFont(ID font, u32 pointSize);

  /// Create a layout with the given text, language, and font
  /// @param text The text
  /// @param language The language
  /// @param fontID The font's ID
  /// @return The pen positions of each glyph in the given text
  std::vector<vec2<float>> shapeText(const std::string& text, const Language& language, ID fontID);

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
