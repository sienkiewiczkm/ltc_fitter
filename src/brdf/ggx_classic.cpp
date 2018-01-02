#include "ggx_classic.hpp"
#include <boost/math/constants/constants.hpp>

ggx_classic::ggx_classic():
  _alpha{0.2f},
  _normal{0.0f, 0.0f, 1.0f}
{
}

float ggx_classic::evaluate(const glm::vec3 &light_dir, const glm::vec3 &view_dir, float &probability_density_function) const
{
  if (view_dir.z <= 0)
  {
    probability_density_function = 0.0f;
    return 0.0f;
  }

  glm::vec3 half_vector = glm::normalize(view_dir + light_dir);

  float ndf = normal_ggx(_normal, half_vector, _alpha);

  float r = std::sqrtf(_alpha) + 1.0f;
  float k = (r * r) / 8.0f;
  float geometry = geometry_smith(_normal, view_dir, light_dir, k);

  float fresnel = 1.0f;

  float nominator = ndf * geometry * fresnel;

  float n_dot_l = std::max(glm::dot(_normal, light_dir), 0.0f);
  float n_dot_v = std::max(glm::dot(_normal, view_dir), 0.0f);
  float denominator = 4.0f * n_dot_l * n_dot_v + 0.001f;

  float n_dot_h = std::max(glm::dot(_normal, half_vector), 0.0f);
  probability_density_function = ndf * n_dot_h;

  return nominator / denominator;
}

glm::vec3 ggx_classic::sample(
  const glm::vec3 &view_dir,
  const glm::vec2 &random_parameters
) const
{
  // based on publications:
  //
  // - Real Shading in Unreal Engine 4
  //    by Brian Karis, Epic Games
  //   from Siggraph 2013
  //   url: https://goo.gl/x8DytH
  //     especially: page 4
  //
  // - Notes on Importance Sampling
  //    by Tobias Alexander Franke
  //   url: https://goo.gl/otR4x1
  //

  const auto pi = boost::math::constants::pi<float>();

  float theta = 2.0f * pi * random_parameters.x;

  auto cos_phi = std::sqrtf((1 - random_parameters.y) / (1 + (_alpha * _alpha - 1) * random_parameters.y));
  auto sin_phi = std::sqrtf(1 - cos_phi * cos_phi);

  glm::vec3 h = {
    std::cos(theta) * sin_phi,
    std::sin(theta) * sin_phi,
    cos_phi
  };

  return glm::reflect(-view_dir, h);
}

float ggx_classic::normal_ggx(glm::vec3 normal, glm::vec3 half_vector, float a) const
{
  float a2 = a * a;
  float n_dot_h = std::max(glm::dot(normal, half_vector), 0.0f);
  float n_dot_h_2 = n_dot_h * n_dot_h;

  const auto pi = boost::math::constants::pi<float>();

  float nominator = a2;
  float denominator = (n_dot_h_2 * (a2 - 1.0f) + 1.0f);
  denominator = pi * denominator * denominator;

  return nominator / denominator;
}

float ggx_classic::geometry_schlick_ggx(float n_dot_v, float k) const
{
  float nominator = n_dot_v;
  float denominator = n_dot_v * (1.0f - k) + k;
  return nominator / denominator;
}

float ggx_classic::geometry_smith(glm::vec3 normal, glm::vec3 view, glm::vec3 light, float k) const
{
  float n_dot_v = std::max(glm::dot(normal, view), 0.0f);
  float n_dot_l = std::max(glm::dot(normal, light), 0.0f);
  float ggx1 = geometry_schlick_ggx(n_dot_l, k);
  float ggx2 = geometry_schlick_ggx(n_dot_v, k);
  return ggx1 * ggx2;
}
