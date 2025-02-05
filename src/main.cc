#include "application.hh"

int main()
{
  Application app(800, 600, 60);

  const std::vector<u8> hackRegularFile = readFile(cwd + "/fonts/Hack-Regular.ttf");
  const std::vector<u8> dejaVuSansFile = readFile(cwd + "/fonts/DejaVuSans.ttf");

  const ID textEntity = app.newEntity("The quick brown fox jumped\nover the lazy doggo", hackRegularFile, "Hack", 32, {}, EnglishLang, {kerningOn});
  app.getText(textEntity).currentColor.fromRGBAf(1, 1, 1, 1);
  app.getText(textEntity).position.x() = -250;
  
  app.addRainbowWaveEffect(textEntity);
  
  app.addJitterEffect(textEntity);
  app.getJitterEffect(textEntity).amount = 2;
  //app.getJitterEffect(textEntity).updateRate = 20;
  
  app.addTextToRender(textEntity);
  app.run();
  return 0;
}
