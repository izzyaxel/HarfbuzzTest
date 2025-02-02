#include "application.hh"
#include "util.hh"
#include "text/langs/textFeatures.hh"

int main()
{
  Application app(800, 600, 60);

  const std::vector<u8> hackRegularFile = readFile(cwd + "/fonts/Hack-Regular.ttf");
  const std::vector<u8> dejaVuSansFile = readFile(cwd + "/fonts/DejaVuSans.ttf");

  glr::Color textColor;
  textColor.fromRGBAf(1, 1, 1, 1);

  const ID dejaVuText = app.textECS.newEntity("The quick brown fox jumped\nover the lazy doggo", dejaVuSansFile, "DejaVu Sans", 32, textColor, EnglishLang, {kerningOn});
  app.textECS.getText(dejaVuText).pos.x() = -200;
  
  app.textECS.addRainbowWaveEffect(dejaVuText);
  
  //app.textECS.addJitterEffect(dejaVuText);
  //app.textECS.getJitterEffect(dejaVuText).updateRate = 20;
  
  app.addTextToRender(dejaVuText);
  app.run();
  return 0;
}
