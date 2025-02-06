#include "application.hh"

constexpr u8 fontSwitch = 1; //0: Hack Regular 1: DejaVu Sans
constexpr u8 colorEffectSwitch = 4; //0: none 1: rainbow 2: solid rainbow 3: solid rainbow fade 4: rainbow wave
constexpr u8 positionEffectSwitch = 2; //0: none 1: jitter 2: wave 3: jitter + wave
constexpr u8 particleEffectSwitch = 1; //0: none 1: sparkles

int main()
{
  Application app(800, 600, 60);
  
  const ID hackRegular = app.newFont(cwd + "/assets/fonts/Hack-Regular.ttf");
  const ID dejaVuSans = app.newFont(cwd + "/assets/fonts/DejaVuSans.ttf");

  const std::string text = "The quick brown fox jumped\nover the lazy doggo";
  constexpr u32 pointSize = 32;
  glr::Color color;
  color.fromRGBAf(1, 1, 1, 1);
  
  ID textEntity = INVALID_ID;

  switch(fontSwitch)
  {
    case 0:
    {
      textEntity = app.newEntity(text, hackRegular, "Hack Regular", pointSize, color, EnglishLang, {kerningOn});
      break;
    }
    case 1:
    {
      textEntity = app.newEntity(text, dejaVuSans, "DejaVu Sans", pointSize, color, EnglishLang, {kerningOn});
      break;
    }
    default: break;
  }
  
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
      app.addParticleEffect(textEntity, 3);
      app.getParticleEffect(textEntity).spawnThreshold = 0.9995f;
      app.getParticleEffect(textEntity).particleLifespan = 0.5f;
      app.getParticleEffect(textEntity).particleLifespanJitter = 0.25f;
      app.getParticleEffect(textEntity).particleScale = {0.3f, 0.3f};
      app.getParticleEffect(textEntity).setParticleTexture("Sparkle", cwd + "/assets/sparkle.png", glr::TexColorFormat::RGBA);
      break;
    }
    default: break;
  }

  app.getText(textEntity).position.x() = -220;
  app.addTextToRender(textEntity);
  app.run();
}
