#pragma once

#include <string>
#include "glm/glm.hpp"

class brdf
{
public:
  virtual ~brdf()
  {
  }

  virtual std::string get_name() const = 0;

  virtual float evaluate(
    const glm::vec3 &light_dir,
    const glm::vec3 &view_dir,
    float &probability_density_function
  ) const = 0;

  virtual glm::vec3 sample(
    const glm::vec3 &view_dir,
    const glm::vec2 &random_parameters
  ) const = 0;

  virtual void set_alpha(float alpha)
  {
  }
};
