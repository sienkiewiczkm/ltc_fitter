#pragma once
#include "sampler2d.hpp"

class uniform_sampler2d: public sampler2d
{
public:
  uniform_sampler2d(glm::ivec2 num_samples): _current_sample{}, _num_samples{num_samples} {}
  virtual ~uniform_sampler2d() = default;

  void reset() override;
  bool isSampleAvailable() override;
  glm::vec2 getNextSample() override;

private:
  glm::ivec2 _current_sample;
  glm::ivec2 _num_samples;
};
