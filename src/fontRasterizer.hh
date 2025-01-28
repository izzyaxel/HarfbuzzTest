#pragma once

#include "alias.hh"
#include "fontData.hh"
#include "langs/textLanguage.hh"

#include <glrAtlas.hh>
#include <commons/math/vec2.hh>

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
  /// @param atlas The atlas to rasterize to
  /// @param atlasTexture The atlas's texture
  void rasterizeFont(ID font, glr::Atlas& atlas, glr::Texture& atlasTexture);

  /// Create a layout with the given text, language, and font
  /// @param text The text
  /// @param language The language
  /// @param fontID The font's ID
  /// @return The pen positions of each glyph in the given text
  std::vector<vec2<float>> shapeText(const std::string& text, const Language& language, ID fontID);

  /// Get the underlying font object from its ID
  /// @param fontID The font to get
  /// @return The font object
  FontData& getFont(ID fontID);

  /// Get the width/height of a glyph for a given font/point size
  /// @param fontID The font
  /// @param glyph The glyph
  /// @return The size of the glyph
  vec2<u32> getGlyphSize(ID fontID, char glyph);

  /// Check if a font has been rasterized and uploaded to the GPU
  /// @param fontID The font
  /// @return Yes/no
  bool isFontRasterized(ID fontID);
  
  private:
  ID lastFontID = 0;
  std::unordered_map<ID, FontData> fonts{};
  FontData nullFont{};
};
