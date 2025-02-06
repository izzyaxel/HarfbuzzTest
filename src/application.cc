#include "application.hh"

#include <SDL2/SDL.h>
#include <glrRenderList.hh>

Application::Application(const u32 width, const u32 height, const u32 targetFPS)
{
  this->targetFPS = targetFPS;
  this->deltaTime.setTargetFinder([this]{return 1.0f / (float)this->targetFPS;});
  
  this->window = std::make_unique<Window>(width, height);
  this->textRenderer = std::make_unique<TextRenderer>();
}

void Application::addTextToRender(const ID text) const
{
  this->textRenderer->addText(text);
}

void Application::run()
{
  while(!this->exiting)
  {
    this->deltaTime.update();
    if(this->deltaTime.isTargetReached())
    {
      this->deltaTime.onTargetReached();
      SDL_Event event;
      while(SDL_PollEvent(&event))
      {
        switch(event.type)
        {
          case SDL_QUIT: //Make the menu bar x button close the window
          {
            this->exiting = true;
            break;
          }
          case SDL_KEYUP:
          {
            if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            {
              this->exiting = true;
            }
            break;
          }
          default: break;
        }
      }
      //Render text
      glr::RenderList graph = this->textRenderer->makeSceneGraph(this->frames, this->deltaTime.deltaF);
      this->window->clearFramebuffer();
      this->window->draw(std::move(graph));
      this->window->swapFramebuffer();
      this->frames++;
    }
  }
}

ID Application::newFont(const std::string &filePath) const
{
  return this->textRenderer->ecs.newFontFile(filePath);
}

ID Application::newEntity(const std::string &text, const ID font, const std::string &fontName, u32 pointSize, glr::Color color, const Language &language, const std::vector<hb_feature_t> &features) const
{
  return this->textRenderer->ecs.newEntity(text, font, fontName, pointSize, color, language, features);
}

void Application::addJitterEffect(const ID entity) const
{
  return this->textRenderer->ecs.addJitterEffect(entity);
}

void Application::addWaveEffect(const ID entity) const
{
  return this->textRenderer->ecs.addWaveEffect(entity);
}

void Application::addRainbowEffect(const ID entity) const
{
  this->textRenderer->ecs.addRainbowEffect(entity);
}

void Application::addRainbowWaveEffect(const ID entity) const
{
  this->textRenderer->ecs.addRainbowWaveEffect(entity);
}

void Application::addParticleEffect(const ID entity, const u32 maxParticles) const
{
  this->textRenderer->ecs.addParticleEffect(entity, maxParticles);
}

void Application::addSolidRainbowEffect(const ID entity) const
{
  this->textRenderer->ecs.addSolidRainbowEffect(entity);
}

void Application::addSolidRainbowFadeEffect(const ID entity) const
{
  this->textRenderer->ecs.addSolidRainbowFadeEffect(entity);
}

TextBlock &Application::getText(const ID entity) const
{
  return this->textRenderer->ecs.getText(entity);
}

JitterEffect &Application::getJitterEffect(const ID entity) const
{
  return this->textRenderer->ecs.getJitterEffect(entity);
}

WaveEffect &Application::getWaveEffect(const ID entity) const
{
  return this->textRenderer->ecs.getWaveEffect(entity);
}

RainbowEffect &Application::getRainbowEffect(const ID entity) const
{
  return this->textRenderer->ecs.getRainbowEffect(entity);
}

RainbowWaveEffect &Application::getRainbowWaveEffect(const ID entity) const
{
  return this->textRenderer->ecs.getRainbowWaveEffect(entity);
}

ParticleEffect &Application::getParticleEffect(const ID entity) const
{
  return this->textRenderer->ecs.getParticleEffect(entity);
}

SolidRainbowEffect &Application::getSolidRainbowEffect(const ID entity) const
{
  return this->textRenderer->ecs.getSolidRainbowEffect(entity);
}

SolidRainbowFadeEffect &Application::getSolidRainbowFadeEffect(const ID entity) const
{
  return this->textRenderer->ecs.getSolidRainbowFadeEffect(entity);
}

bool Application::hasJitterEffect(const ID entity) const
{
  return this->textRenderer->ecs.hasJitterEffect(entity);
}

bool Application::hasWaveEffect(const ID entity) const
{
  return this->textRenderer->ecs.hasWaveEffect(entity);
}

bool Application::hasRainbowEffect(const ID entity) const
{
  return this->textRenderer->ecs.hasRainbowEffect(entity);
}

bool Application::hasRainbowWaveEffect(const ID entity) const
{
  return this->textRenderer->ecs.hasRainbowWaveEffect(entity);
}

bool Application::hasSolidRainbowEffect(const ID entity) const
{
  return this->textRenderer->ecs.hasSolidRainbowEffect(entity);
}

bool Application::hasSolidRainbowFadeEffect(const ID entity) const
{
  return this->textRenderer->ecs.hasSolidRainbowFadeEffect(entity);
}

bool Application::hasParticleEffect(const ID entity) const
{
  return this->textRenderer->ecs.hasParticleEffect(entity);
}
