#pragma once

#include "text/fontRasterizer.hh"
#include "window.hh"

struct Application
{
  Application(u32 width, u32 height);
  
  void run();
  
  Window window;
  FontRasterizer fontRasterizer;

  bool exiting = false;
};
