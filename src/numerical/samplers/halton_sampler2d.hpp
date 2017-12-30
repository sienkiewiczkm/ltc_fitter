#pragma once
#include "sampler2d.hpp"

class halton_sampler2d: public sampler2d
{
public:
  halton_sampler2d(): _currentIndex{0} {}
  ~halton_sampler2d() override {}

  void reset() override;
  bool isSampleAvailable() override;
  glm::vec2 getNextSample() override;

private:
  uint32_t _currentIndex;
};
