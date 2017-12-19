#pragma once
#include "glm/glm.hpp"

/*
 * This class is a hub for all stupid hacks that should be removed at some point,
 * Just find an usage, figure how to do it properly and fix it asap!
 */
class hacks
{
public:
  hacks() = default;
  static glm::vec3 fix_parameters_below_zero(const glm::vec3 &parameters);
};
