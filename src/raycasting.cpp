#include "raycasting.hpp"
#include <algorithm>

bool ray_sphere_min_intersection(
  glm::vec3 origin,
  glm::vec3 direction,
  glm::vec3 sphere_origin,
  float sphere_radius,
  float &intersection_distance
)
{
  auto l = sphere_origin - origin;
  auto l_len = glm::length(l);

  auto tc = glm::dot(l, direction);

  if (tc < 0.0f)
  {
    return false;
  }

  float d2 = (l_len * l_len) - (tc * tc);
  float r2 = sphere_radius * sphere_radius;

  if (d2 > r2)
  {
    return false;
  }

  float t1c = sqrtf(r2 - d2);

  float t1 = tc - t1c;
  float t2 = tc + t1c;

  intersection_distance = std::min(t1, t2);
  return true;
}
