#include <boost/math/constants/constants.hpp>
#include "ggx.hpp"
#include "glm/glm.hpp"

// Alternate forms were (to my knowledge) created by Eric Heitz. See his Linearly Transformed Cosines paper for more
// details (and papers listed below in code).

std::string ggx::get_name() const
{
  return "ggx";
}

float ggx::evaluate(
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

  float g2 = calculate_shadow_masking_term(light_dir, view_dir);

  const glm::vec3 h = glm::normalize(view_dir + light_dir);
  float ndf = calculateNDF(h);

  // Additional factor in the pdf (1/(4 dot(v,h))) is explained here:
  // - Microfacet Models for Refraction through Rough Surfaces
  // - Notes on the Ward BRDF, Bruce Walter
  probability_density_function = fabsf(ndf * h.z / 4.0f / glm::dot(view_dir, h));

  // We assume that Fresnel factor is equal to 1.
  float res = ndf * g2 / 4.0f / view_dir.z;

  return res;
}

float ggx::calculateNDF(const glm::vec3 &h) const
{
  // tan(theta) = sqrt(x^2 + y^2) / z
  // tan(theta)^2 = (x^2/z^2) + (y^2/z^2) = (x/z)^2 + (y/z)^2
  const float slope_x = h.x/h.z;
  const float slope_y = h.y/h.z;
  const float tan_theta_sq = (slope_x*slope_x + slope_y*slope_y);

  // Alternate form is result of dividing by alpha^2 inside parentheses in denominator in the GGX classic form.
  // This form is also equal to Disney form:
  //   cos(theta)^4 (alpha^2 + tan(theta)^2) = (alpha^2 cos(theta)^2 + sin(theta)^2)^2 =
  //     = (alpha^2 cos(theta)^2 + 1 - cos(theta)^2) = (cos(theta)^2 (alpha^2 - 1) + 1)^2

  float ndf = 1.0f / (1.0f + tan_theta_sq/_alpha/_alpha);
  ndf = (ndf*ndf) / (3.14159f * _alpha * _alpha * h.z*h.z*h.z*h.z);

  return ndf;
}

glm::vec3 ggx::sample(const glm::vec3 &view_dir, const glm::vec2 &random_parameters) const
{
  float u1 = random_parameters.x;
  float u2 = random_parameters.y;

  // This is classic GGX sampling, it is rewritten to solve for X-axis side of right triangle. For simplicity,
  // we assume that other side is equal to 1. Hypotenuse is equal to sqrt(r^2+1). In that case:
  //   cos(theta) = (1) / sqrt(r^2+1)
  // Now we can transform standard GGX sampling method by comparing that cos(theta) and cos(theta) defined by
  // sampling method. In the end we get:
  //   r = alpha * sqrt(u/(1-u))

  const auto pi = boost::math::constants::pi<float>();
  const float phi = 2.0f * pi * u1;
  const float r = _alpha * sqrtf(u2/(1.0f - u2));

  const glm::vec3 h = glm::normalize(glm::vec3(r*cosf(phi), r*sinf(phi), 1.0f));
  const glm::vec3 l = glm::reflect(-view_dir, h);

  return l;
}

float ggx::calculate_shadow_masking_term(const glm::vec3 &light_dir, const glm::vec3 &view_dir) const
{
  // See for more details:
  // - Understanding the Masking-Shadowing Function in Microfacet-Based BRDFs
  // - Microfacet Models for Refraction through Rough Surfaces
  // - PBR Diffuse Lighting for GGX+Smith Microsurfaces (Slides)

  if (light_dir.z <= 0.0f)
  {
    return 0.0f;
  }

  const auto lambda_v = lambda(view_dir.z);
  const auto lambda_l = lambda(light_dir.z);

  auto g2 = 1.0f / (1.0f + lambda_v + lambda_l);

  return g2;
}

float ggx::lambda(const float cosTheta) const
{
  const float a = 1.0f / _alpha / tanf(acosf(cosTheta));
  return (cosTheta < 1.0f) ? 0.5f * (-1.0f + sqrtf(1.0f + 1.0f/a/a)) : 0.0f;
}
