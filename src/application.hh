#pragma once

#include "deltaTime.hh"
#include "textEcs.hh"
#include "text/textBlock.hh"
#include "text/langs/textFeatures.hh"
#include "window.hh"

struct Application
{
  Application(u32 width, u32 height, u32 targetFPS);

  void addTextToRender(ID text);
  void run();

  TextECS ecs;
  Window window;
  bool exiting = false;
  u32 targetFPS = 0;
  u64 frames = 0;
  DeltaTime deltaTime{[] { return 0.0f; }};
  
  private:
  std::vector<ID> textToRender;
};
