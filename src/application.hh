#pragma once

#include "deltaTime.hh"
#include "text/textBlock.hh"
#include "window.hh"

struct Application
{
  Application(u32 width, u32 height, u32 targetFPS);

  void addText(TextBlock text);
  void run();
  
  Window window;

  bool exiting = false;
  u32 targetFPS = 0;

  u64 frames = 0;

  private:
  std::vector<TextBlock> textToRender;
  DeltaTime deltaTime{[] { return 0.0f; }};
};
