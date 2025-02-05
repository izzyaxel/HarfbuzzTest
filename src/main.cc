#include "application.hh"

int main()
{
  Application app(800, 600, 60);
  const ID hackRegular = app.newFont(cwd + "/fonts/Hack-Regular.ttf");
  const ID dejaVuSans = app.newFont(cwd + "/fonts/DejaVuSans.ttf");
  const ID textEntity = app.newEntity("The quick brown fox jumped\nover the lazy doggo", hackRegular, "Hack", 32, {}, EnglishLang, {kerningOn});
  app.getText(textEntity).currentColor.fromRGBAf(1, 1, 1, 1);
  app.getText(textEntity).position.x() = -250;
  app.addWaveEffect(textEntity);
  app.addRainbowWaveEffect(textEntity);
  //app.addJitterEffect(textEntity);
  app.addTextToRender(textEntity);
  app.run();
}
