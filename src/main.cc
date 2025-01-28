#include "fontRasterizer.hh"
#include "util.hh"
#include "openGl.hh"

#include <glrAtlas.hh>

int main()
{
  FontRasterizer fontRasterizer;
  const ID hackID = fontRasterizer.addFont(cwd + "/fonts/Hack-Regular.ttf", "Hack Regular", 24);
  const ID dejaVuID = fontRasterizer.addFont(cwd + "/fonts/DejaVuSans.ttf", "DejaVu Sans", 32);
  if(hackID == INVALID_ID || dejaVuID == INVALID_ID)
  {
    return -1;
  }

  //TODO Create OpenGL context
  startOpenGL();
  glr::Atlas hackAtlas = {};
  glr::Atlas dejaVuAtlas = {};
  glr::Texture hackTexture = {};
  glr::Texture dejaVuTexture = {};
  
  fontRasterizer.rasterizeFont(hackID, hackAtlas, hackTexture);
  fontRasterizer.rasterizeFont(dejaVuID, dejaVuAtlas, dejaVuTexture);
  if(!fontRasterizer.isFontRasterized(hackID) || !fontRasterizer.isFontRasterized(dejaVuID))
  {
    return -1;
  }
  
  const std::vector<vec2<float>> penPositionsHack = fontRasterizer.shapeText("How now brown cow", EnglishLang, hackID);
  const std::vector<vec2<float>> penPositionsDejaVu = fontRasterizer.shapeText("The quick brown fox", EnglishLang, dejaVuID);
  if(penPositionsHack.empty() || penPositionsDejaVu.empty())
  {
    return -1;
  }
  
  printf("Success\n");
  
  return 0;
}
