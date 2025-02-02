#include "application.hh"

int main()
{
  Application app(800, 600, 60);

  const std::vector<u8> hackRegularFile = readFile(cwd + "/fonts/Hack-Regular.ttf");
  const std::vector<u8> dejaVuSansFile = readFile(cwd + "/fonts/DejaVuSans.ttf");

  const ID text = app.ecs.newEntity("The quick brown fox jumped\nover the lazy doggo", hackRegularFile, "Hack", 32, {}, EnglishLang, {kerningOn});
  app.ecs.getText(text).currentColor.fromRGBAf(1, 1, 1, 1);
  app.ecs.getText(text).position.x() = -200;
  
  app.ecs.addRainbowWaveEffect(text);
  
  //app.ecs.addJitterEffect(dejaVuText);
  //app.ecs.getJitterEffect(dejaVuText).amount = 2;
  //app.ecs.getJitterEffect(dejaVuText).updateRate = 20;
  
  app.addTextToRender(text);
  app.run();
  return 0;
}
