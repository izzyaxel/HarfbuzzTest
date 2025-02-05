#pragma once
#include "glrRenderList.hh"
#include "../alias.hh"
#include "textEcs.hh"

struct TextRenderer
{
  TextRenderer();
  
  void addText(ID entity);
  glr::RenderList makeSceneGraph(u64 frames, float deltaTime);

  TextECS ecs;

  private:
  std::vector<ID> textToRender{};
};
