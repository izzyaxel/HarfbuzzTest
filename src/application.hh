#pragma once

#include "text/fontRasterizer.hh"
#include "window.hh"
#include "text/text.hh"

struct Application
{
  Application(u32 width, u32 height);

  void addText(const Text& text);
  void run();
  
  Window window;
  FontRasterizer fontRasterizer;

  bool exiting = false;

  private:
  std::vector<Text> textToRender;
};
