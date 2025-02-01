#pragma once

#include "alias.hh"

#include <string>
#include <vector>
#include <filesystem>
#include <cstdlib>
#include <random>

inline constexpr ID INVALID_ID = std::numeric_limits<ID>::max();
inline constexpr float SCALAR = 64.0f;
inline std::default_random_engine generator;
inline std::string cwd = std::filesystem::current_path().string();

std::vector<u8> readFile(const std::string& filePath);
float randomFloat(float min, float max);

/// 
/// @param hsv Hue saturation value; hue is 0-360, s & v are 0-1
/// @return 0-1 RGB
vec3<float> hsvTorgb(const vec3<float>& hsv);
