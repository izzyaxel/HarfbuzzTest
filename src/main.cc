#include "application.hh"
#include "text/text.hh"

int main()
{
  Application app(800, 600);

  const ID hackRegularFileID = app.fontRasterizer.loadFont(cwd + "/fonts/Hack-Regular.ttf", "Hack Regular");
  const ID dejaVuSansFileID = app.fontRasterizer.loadFont(cwd + "/fonts/DejaVuSans.ttf", "DejaVu Sans");

  //TODO FIXME If multiple fonts are rasterized consecutively, then shaping text afterwards segfaults on hb_shape, but shaping text directly after a single font is rasterized doesn't segfault
  //TODO FIXME IE "rasterize rasterize shape shape" segfaults, but "rasterize shape rasterize shape" does not segfault
  //TODO FIXME This seems to be due to harfbuzz or freetype objects being free'd and reused later, but I haven't been able to find anywhere where that happens yet
  
  //Test 1
  {
    Text hack24ptText{24, "How now brown cow", EnglishLang};
    app.fontRasterizer.rasterizeFont(hackRegularFileID, hack24ptText);
    app.fontRasterizer.shapeText(hack24ptText);
    app.addText(hack24ptText);
  }
  
  //Test 2
  {
    Text dejaVu32ptText{32, "The quick brown fox", EnglishLang};
    app.fontRasterizer.rasterizeFont(dejaVuSansFileID, dejaVu32ptText);
    app.fontRasterizer.shapeText(dejaVu32ptText);
    app.addText(dejaVu32ptText);
  }

  //Test 3
  {
    Text dejaVu16ptText{16, "jumped over the lazy doggo", EnglishLang};
    app.fontRasterizer.rasterizeFont(dejaVuSansFileID, dejaVu16ptText);
    app.fontRasterizer.shapeText(dejaVu16ptText);
    app.addText(dejaVu16ptText);
  }
  
  app.run();
  return 0;
}
