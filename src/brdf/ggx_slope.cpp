#include "ggx_slope.hpp"
#include "glm/glm.hpp"

std::string ggx_slope::get_name() const
{
  return "ggx_slope";
}

float ggx_slope::evaluate(
  const glm::vec3 &light_dir,
  const glm::vec3 &view_dir,
  float &probability_density_function
) const
{
  if (view_dir.z <= 0)
  {
    probability_density_function = 0;
    return 0;
  }

  // masking
  const float LambdaV = lambda(view_dir.z);

  // shadowing
  float G2;
  if (light_dir.z <= 0.0f)
  {
    G2 = 0;
  }
  else
  {
    const float LambdaL = lambda(light_dir.z);
    G2 = 1.0f/(1.0f + LambdaV + LambdaL);
  }

  // D
  const glm::vec3 H = glm::normalize(view_dir + light_dir);
  const float slopex = H.x/H.z;
  const float slopey = H.y/H.z;

  float D = 1.0f / (1.0f + (slopex*slopex + slopey*slopey)/_alpha/_alpha);
  D = D*D;
  D = D/(3.14159f * _alpha*_alpha * H.z*H.z*H.z*H.z);

  probability_density_function = fabsf(D * H.z / 4.0f / glm::dot(view_dir, H));
  float res = D * G2 / 4.0f / view_dir.z;

  return res;
}

glm::vec3 ggx_slope::sample(const glm::vec3 &view_dir, const glm::vec2 &random_parameters) const
{
  float U1 = random_parameters.x;
  float U2 = random_parameters.y;
  const float phi = 2.0f*3.14159f * U1;
  const float r = _alpha*sqrtf(U2/(1.0f - U2));
  const glm::vec3 N = glm::normalize(glm::vec3(r*cosf(phi), r*sinf(phi), 1.0f));
  const glm::vec3 L = -view_dir + 2.0f * N * dot(N, view_dir);
  return L;
}

float ggx_slope::lambda(const float cosTheta) const
{
  const float a = 1.0f / _alpha / tanf(acosf(cosTheta));
  return (cosTheta < 1.0f) ? 0.5f * (-1.0f + sqrtf(1.0f + 1.0f/a/a)) : 0.0f;
}
