#pragma once

#include "alias.hh"

enum struct PNGColorFmt
{
  GREY = 0,
  PALETTE = 2 | 1,
  RGB = 2,
  RGBA = 2 | 4,
  GREY_ALPHA = 4,
};

struct PNG
{
  PNG(u32 width, u32 height, char colorFormat, char bitDepth, std::vector<u8> &&imageData);

  ///The width and height of the image in pixels
  u32 width;
  u32 height;

  ///The color format and depth width the decoded data is in
  char colorFormat;
  char bitDepth;

  /// The decoded pixel data
  std::vector<u8> imageData;
};

/// Read and decode a PNG image
[[nodiscard]] PNG decodePNG(const std::string& filePath);

/// Decode a PNG image
[[nodiscard]] PNG decodePNG(const std::vector<u8>& file);
