#pragma once

#include "brdf.hpp"

class ggx: public brdf
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
  float calculate_shadow_masking_term(const glm::vec3 &light_dir, const glm::vec3 &view_dir) const;
  float lambda(const float cosTheta) const;

  float _alpha;
  float calculateNDF(const glm::vec3 &h) const;
};
