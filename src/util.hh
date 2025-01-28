#pragma once

#include "alias.hh"

#include <string>
#include <vector>
#include <limits>
#include <filesystem>

constexpr static ID INVALID_ID = std::numeric_limits<ID>::max();
constexpr static float SCALAR = 64.0f;
constexpr static u32 LINE_SPACING = 10;

inline std::string cwd = std::filesystem::current_path().string();

template <typename T>
struct vec2
{
  vec2() = default;
  vec2(T x, T y) : a(x), b(y) {}
  
  T& x()
  {
    return this->a;
  }

  T& y()
  {
    return this->b;
  }

  private:
  T a, b;
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
