#pragma once

#include "text/textBlock.hh"
#include "window.hh"

struct Application
{
  Application(u32 width, u32 height);

  void addText(TextBlock text);
  void run();
  
  Window window;

  bool exiting = false;

  private:
  std::vector<TextBlock> textToRender;
};
