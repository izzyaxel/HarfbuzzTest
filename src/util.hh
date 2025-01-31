#pragma once

#include "alias.hh"

#include <string>
#include <vector>
#include <filesystem>
#include <cstdlib>
#include <random>

inline constexpr float SCALAR = 64.0f;
inline std::default_random_engine generator;
inline std::string cwd = std::filesystem::current_path().string();

std::vector<u8> readFile(const std::string& filePath);
float randomFloat(float min, float max);
