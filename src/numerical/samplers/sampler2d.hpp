#pragma once
#include <glm/glm.hpp>

class sampler2d
{
public:
  virtual ~sampler2d() {}
  virtual void reset() = 0;
  virtual bool isSampleAvailable() = 0;
  virtual glm::vec2 getNextSample() = 0;
};
