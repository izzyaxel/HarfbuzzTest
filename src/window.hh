#pragma once
#include "alias.hh"
#include "glrRenderList.hh"

struct Window
{
  Window(u32 width, u32 height);
  ~Window();

  void draw(glr::RenderList&& renderList);
  void clearFramebuffer() const;
  void swapFramebuffer() const;

private:
  void* window = nullptr;
  void* context = nullptr;
  void* renderer = nullptr;
};
