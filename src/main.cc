#include "application.hh"

int main()
{
  Application app(800, 600, 60);

  const std::vector<u8> hackRegularFile = readFile(cwd + "/fonts/Hack-Regular.ttf");
  const std::vector<u8> dejaVuSansFile = readFile(cwd + "/fonts/DejaVuSans.ttf");

  const ID dejaVuText = app.ecs.newEntity("The quick brown fox jumped\nover the lazy doggo", dejaVuSansFile, "DejaVu Sans", 32, {}, EnglishLang, {kerningOn});
  app.ecs.getText(dejaVuText).currentColor.fromRGBAf(1, 1, 1, 1);
  app.ecs.getText(dejaVuText).position.x() = -200;
  
  app.ecs.addRainbowWaveEffect(dejaVuText);
  
  //app.ecs.addJitterEffect(dejaVuText);
  //app.ecs.getJitterEffect(dejaVuText).amount = 2;
  //app.ecs.getJitterEffect(dejaVuText).updateRate = 20;
  
  app.addTextToRender(dejaVuText);
  app.run();
  return 0;
}
