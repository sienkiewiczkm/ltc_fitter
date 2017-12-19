#include "hacks.hpp"
#include <algorithm>

glm::vec3 hacks::fix_parameters_below_zero(const glm::vec3 &parameters)
{
  float min_value = 1e-6;
  auto new_parameters = parameters;
  new_parameters.x = std::max(min_value, new_parameters.x);
  new_parameters.y = std::max(min_value, new_parameters.y);
  return new_parameters;
}
