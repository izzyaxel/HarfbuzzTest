#include "config/textLanguage.hh"

#include <harfbuzz/hb-ft.h>
#include <harfbuzz/hb.h>

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

using u8 = uint8_t;
using u32 = uint32_t;
using u64 = uint64_t;
using ID = u64;

constexpr static ID INVALID_ID = std::numeric_limits<ID>::max();
constexpr static float POINTSIZE = 32.0f;
constexpr static float SCALAR = 64.0f;
constexpr static long SCALEDPS = (long)(POINTSIZE * SCALAR);

template <typename T>
struct vec2
{
  T x, y;
};

struct FontData
{
  FT_Library ftLib = nullptr;
  FT_Face ftFace = nullptr;
  hb_font_t* hbFont = nullptr;
  u32 fontPointSize = 0;
  u32 tallestGlyph = 0;
  std::unordered_map<char, vec2<u32>> glyphSizes{};
  bool ready = false;
};

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

size_t addFont(std::unordered_map<ID, FontData>& fontMap, const std::vector<u8>& fontData)
{
  const size_t index = fontMap.size();
  fontMap[index] = {};
  
  //Start FreeType
  FT_Error ftError = FT_Init_FreeType(&fontMap.at(index).ftLib);
  if(ftError != 0 || !fontMap.at(index).ftLib)
  {
    return INVALID_ID;
  }

  //Create FreeType font
  ftError = FT_New_Memory_Face(fontMap.at(index).ftLib, fontData.data(), (long)fontData.size(), 0, &fontMap.at(index).ftFace);
  if(ftError != 0)
  {
    return INVALID_ID;
  }

  //Set FreeType font size
  ftError = FT_Set_Char_Size(fontMap.at(index).ftFace, SCALEDPS, SCALEDPS, 0, 0);
  if(ftError != 0)
  {
    return INVALID_ID;
  }

  //Create HarfBuzz font from the FreeType face
  fontMap.at(index).hbFont = hb_ft_font_create_referenced(fontMap.at(index).ftFace);
  if(!fontMap.at(index).hbFont)
  {
    return INVALID_ID;
  }
  return index;
}

void shapeText(const std::string& text, const Language& language, const FontData& fontData)
{
  hb_buffer_t* hbBuffer = hb_buffer_create();
  hb_buffer_set_script(hbBuffer, language.script);
  hb_buffer_set_direction(hbBuffer, language.direction);
  hb_buffer_set_language(hbBuffer, hb_language_from_string(language.lang.c_str(), -1));
  hb_buffer_add_utf8(hbBuffer, text.c_str(), -1, 0, -1);
  hb_font_t* hbFont = hb_font_reference(fontData.hbFont);
  hb_shape(hbFont, hbBuffer, nullptr, 0);
  if(hb_buffer_get_content_type(hbBuffer) != HB_BUFFER_CONTENT_TYPE_GLYPHS)
  {
    hb_font_destroy(hbFont);
    printf("Buffer failed to be shaped\n");
    return;
  }
  hb_font_destroy(hbFont);
  hb_buffer_destroy(hbBuffer);
}

int main()
{
  const Language eng = EnglishLang;
  const std::string textA = "How now brown cow";
  const std::string textB = "The quick brown fox";
  std::unordered_map<ID, FontData> fonts{};
  
  const std::vector<u8> fontDataHack = readFile(std::filesystem::current_path().string() + "/fonts/Hack-Regular.ttf");
  if(fontDataHack.empty())
  {
    return -1;
  }

  const std::vector<u8> fontDataDejaVu = readFile(std::filesystem::current_path().string() + "/fonts/DejaVuSans.ttf");
  if(fontDataDejaVu.empty())
  {
    return -1;
  }
  
  const size_t hackID = addFont(fonts, fontDataHack);
  if(hackID == INVALID_ID)
  {
    return -1;
  }

  const size_t dejaVuID = addFont(fonts, fontDataDejaVu);
  if(dejaVuID == INVALID_ID)
  {
    return -1;
  }
  
  shapeText(textA, eng, fonts.at(hackID));
  shapeText(textB, eng, fonts.at(dejaVuID));
  
  printf("Success\n");
  
  return 0;
}
