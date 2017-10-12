#pragma once

#include "brdf.hpp"

/* This class is taken almost as-is from ltc_code fitter project by Eric Heitz. */
class ggx_slope: public brdf
{
public:
  std::string get_name() const override;

  float evaluate(
    const glm::vec3 &light_dir,
    const glm::vec3 &view_dir,
    float &probability_density_function
  ) const override;

  glm::vec3 sample(const glm::vec3 &view_dir, const glm::vec2 &random_parameters) const override;

  void set_alpha(float alpha) override
  {
    _alpha = alpha;
  }

private:
  float lambda(const float cosTheta) const;

  float _alpha;
};
