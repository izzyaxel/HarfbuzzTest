#include "util.hh"

std::vector<u8> readFile(const std::string& filePath)
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

float randomFloat(const float min, const float max)
{
  std::uniform_real_distribution distribution(min, max);
  return distribution(generator);
}

vec3<float> hsvTorgb(const vec3<float>& hsv)
{
  vec3<float> out{};
  const float h = hsv.r() / 360.0f;
  const float s = hsv.g();
  const float v = hsv.b();
  
  const i32 i = std::floor(h * 6);
  
  const float f = h * 6 - i;
  const float p = v * (1 - s);
  const float q = v * (1 - f * s);
  const float t = v * (1 - (1 - f) * s);
  
  switch(i % 6)
  {
    case 0:
    {
      out.r() = v;
      out.g() = t;
      out.b() = p;
      break;
    }
    case 1:
    {
      out.r() = q;
      out.g() = v;
      out.b() = p;
      break;
    }
    case 2:
    {
      out.r() = p;
      out.g() = v;
      out.b() = t;
      break;
    }
    case 3:
    {
      out.r() = p;
      out.g() = q;
      out.b() = v;
      break;
    }
    case 4:
    {
      out.r() = t;
      out.g() = p;
      out.b() = v;
      break;
    }
    case 5:
    {
      out.r() = v;
      out.g() = p;
      out.b() = q;
      break;
    }
    default: break;
  }
  return out;
}
