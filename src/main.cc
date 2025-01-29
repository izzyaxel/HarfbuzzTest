#include "application.hh"

int main()
{
  Application app(800, 600);

  const ID hackRegular = app.fontRasterizer.loadFont(cwd + "/fonts/Hack-Regular.ttf", "Hack Regular");
  const ID dejaVuSans = app.fontRasterizer.loadFont(cwd + "/fonts/DejaVuSans.ttf", "DejaVu Sans");

  //TODO FIXME If multiple fonts are rasterized consecutively, then shaping text afterwards segfaults on hb_shape, but shaping text directly after a single font is rasterized doesn't segfault
  //TODO FIXME IE "rasterize rasterize shape shape" segfaults, but "rasterize shape rasterize shape" does not segfault
  //TODO FIXME This seems to be due to harfbuzz or freetype objects being free'd and reused later, but I haven't been able to find anywhere where that happens yet
  
  //Test 1
  {
    const ID hack24ptRasterID = app.fontRasterizer.rasterizeFont(hackRegular, 24);
    if(hack24ptRasterID == INVALID_ID || !app.fontRasterizer.isFontRasterized(hack24ptRasterID))
    {
      return -1;
    }
    const std::vector<vec2<float>> penPositionsHack24pt = app.fontRasterizer.shapeText("How now brown cow", EnglishLang, hack24ptRasterID);
    if(penPositionsHack24pt.empty())
    {
      return -1;
    }
  }
  
  //Test 2
  {
    const ID dejaVu32ptRasterID = app.fontRasterizer.rasterizeFont(dejaVuSans, 32);
    if(dejaVu32ptRasterID == INVALID_ID || !app.fontRasterizer.isFontRasterized(dejaVu32ptRasterID))
    {
      return -1;
    }
    const std::vector<vec2<float>> penPositionsDejaVu32pt = app.fontRasterizer.shapeText("The quick brown fox", EnglishLang, dejaVu32ptRasterID);
    if(penPositionsDejaVu32pt.empty())
    {
      return -1;
    }
  }

  //Test 3
  {
    const ID dejaVu16ptRasterID = app.fontRasterizer.rasterizeFont(dejaVuSans, 16);
    if(dejaVu16ptRasterID == INVALID_ID || !app.fontRasterizer.isFontRasterized(dejaVu16ptRasterID))
    {
      return -1;
    }
    const std::vector<vec2<float>> penPositionsDejaVu16pt = app.fontRasterizer.shapeText("jumped over the lazy doggo", EnglishLang, dejaVu16ptRasterID);
    if(penPositionsDejaVu16pt.empty())
    {
      return -1;
    }
  }
  
  app.run();
  return 0;
}
