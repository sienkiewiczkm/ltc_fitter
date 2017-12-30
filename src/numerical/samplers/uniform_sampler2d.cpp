#include "uniform_sampler2d.hpp"
#include <stdexcept>

void uniform_sampler2d::reset()
{
  _current_sample = {0, 0};
}

bool uniform_sampler2d::isSampleAvailable()
{
  return _current_sample.y < _num_samples.y;
}

glm::vec2 uniform_sampler2d::getNextSample()
{
  if (!isSampleAvailable())
  {
    throw std::logic_error("Next sample is not available.");
  }

  glm::vec2 sample{
    (_current_sample.x + 0.5f) / (_num_samples.x - 1),
    (_current_sample.y + 0.5f) / (_num_samples.y - 1)
  };

  ++_current_sample.x;
  if (_current_sample.x >= _num_samples.x) {
    _current_sample.x = 0;
    ++_current_sample.y;
  }

  return sample;
}
