#include "application.hh"

constexpr u8 fontSwitch = 1; //0: Hack Regular 1: DejaVu Sans
constexpr u8 colorEffectSwitch = 4;
constexpr u8 positionEffectSwitch = 2;
constexpr u8 particleEffectSwitch = 1;

int main()
{
  Application app(800, 600, 60);
  
  const ID hackRegular = app.newFont(cwd + "/assets/fonts/Hack-Regular.ttf");
  const ID dejaVuSans = app.newFont(cwd + "/assets/fonts/DejaVuSans.ttf");
  
  ID textEntity = INVALID_ID;

  switch(fontSwitch)
  {
    case 0:
    {
      textEntity = app.newEntity("The quick brown fox jumped\nover the lazy doggo", hackRegular, "Hack Regular", 32, {}, EnglishLang, {kerningOn});
      break;
    }
    case 1:
    {
      textEntity = app.newEntity("The quick brown fox jumped\nover the lazy doggo", dejaVuSans, "DejaVu Sans", 32, {}, EnglishLang, {kerningOn});
      break;
    }
    default: break;
  }
  
  app.getText(textEntity).position.x() = -220;
  
  switch(colorEffectSwitch)
  {
    case 1:
    {
      app.addRainbowEffect(textEntity);
      app.getRainbowEffect(textEntity).updateRate = 10;
      break;
    }
    case 2:
    {
      app.addSolidRainbowEffect(textEntity);
      app.getSolidRainbowEffect(textEntity).updateRate = 10;
      break;
    }
    case 3:
    {
      app.addSolidRainbowFadeEffect(textEntity);
      break;
    }
    case 4:
    {
      app.addRainbowWaveEffect(textEntity);
      break;
    }
    default: break;
  }

  switch(positionEffectSwitch)
  {
    case 1:
    {
      app.addJitterEffect(textEntity);
      app.getJitterEffect(textEntity).amount = 2;
      break;
    }
    case 2:
    {
      app.addWaveEffect(textEntity);
      break;
    }
    case 3:
    {
      app.addJitterEffect(textEntity);
      app.getJitterEffect(textEntity).amount = 2;
      app.addWaveEffect(textEntity);
      break;
    }
    default: break;
  }

  switch(particleEffectSwitch)
  {
    case 1:
    {
      app.addParticleEffect(textEntity, 100);
      app.getParticleEffect(textEntity).setParticleTexture("Sparkle", cwd + "/assets/sparkle.png", glr::TexColorFormat::RGBA);
      app.getParticleEffect(textEntity).particleScale = {0.3f, 0.3f};
      break;
    }
    default: break;
  }
  
  app.addTextToRender(textEntity);
  app.run();
}
