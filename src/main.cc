#include "langs/textLanguage.hh"

#include <filesystem>
#include <string>
#include <vector>

#include "alias.hh"
#include "fontRasterizer.hh"
#include "util.hh"

int main()
{
  FontRasterizer fontRasterizer;
  const std::vector<u8> fontDataHack = readFile(std::filesystem::current_path().string() + "/fonts/Hack-Regular.ttf");
  const std::vector<u8> fontDataDejaVu = readFile(std::filesystem::current_path().string() + "/fonts/DejaVuSans.ttf");
  if(fontDataHack.empty() || fontDataDejaVu.empty())
  {
    return -1;
  }
  
  const ID hackID = fontRasterizer.addFont(fontDataHack, 24);
  const ID dejaVuID = fontRasterizer.addFont(fontDataDejaVu, 24);
  if(hackID == INVALID_ID || dejaVuID == INVALID_ID)
  {
    return -1;
  }
  
  fontRasterizer.shapeText("How now brown cow", EnglishLang, hackID);
  fontRasterizer.shapeText("The quick brown fox", EnglishLang, dejaVuID);
  
  printf("Success\n");
  
  return 0;
}
