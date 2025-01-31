#include "application.hh"
#include "util.hh"
#include "text/langs/textFeatures.hh"

int main()
{
  Application app(800, 600, 60);

  const std::vector<u8> hackRegularFile = readFile(cwd + "/fonts/Hack-Regular.ttf");
  const std::vector<u8> dejaVuSansFile = readFile(cwd + "/fonts/DejaVuSans.ttf");

  glr::Color textColor;
  textColor.fromRGBAf(0, 0, 0, 1);
  
  TextBlock dejaVu32PtText("The quick brown fox jumped\nover the lazy doggo", dejaVuSansFile, "DejaVu Sans", 32, textColor, EnglishLang, {kerningOn, ligaturesOn, contextualLigaturesOn});
  dejaVu32PtText.addTextEffects({TextEffect::JITTER, TextEffect::RAINBOW});
  dejaVu32PtText.rainbowUpdateRate = 10;
  dejaVu32PtText.jitterUpdateRate = 1;
  dejaVu32PtText.jitterAmount = 5;
  
  app.addText(std::move(dejaVu32PtText));
  app.run();
  return 0;
}
