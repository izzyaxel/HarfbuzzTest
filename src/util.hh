#pragma once

#include "alias.hh"

#include <string>
#include <vector>
#include <limits>

constexpr static ID INVALID_ID = std::numeric_limits<ID>::max();
constexpr static float SCALAR = 64.0f;

template <typename T>
struct vec2
{
  T x, y;
};

inline std::vector<u8> readFile(const std::string& filePath)
{
  FILE* in = fopen(filePath.c_str(), "rb");
  if(!in)
  {
    return {};
  }
  std::vector<u8> out;
  fseek(in, 0, SEEK_END);
  const size_t fileSize = ftell(in);
  rewind(in);
  out.resize(fileSize);
  fread(out.data(), fileSize, 1, in);
  fclose(in);
  return out;
}
