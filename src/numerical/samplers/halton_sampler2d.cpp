#include "halton_sampler2d.hpp"
#include "van_der_corput.hpp"

void halton_sampler2d::reset()
{
  _currentIndex = 0;
}

bool halton_sampler2d::isSampleAvailable()
{
  return true;
}

glm::vec2 halton_sampler2d::getNextSample()
{
  glm::vec2 sample{
    van_der_corput(_currentIndex, 2),
    van_der_corput(_currentIndex, 3)
  };

  ++_currentIndex;

  return sample;
}
