#pragma once
#include "alias.hh"

struct Window
{
  Window(u32 width, u32 height);
  ~Window();

  void clearFramebuffer() const;
  void swapFramebuffer() const;

private:
  void* window = nullptr;
  void* context = nullptr;
  void* renderer = nullptr;
};
