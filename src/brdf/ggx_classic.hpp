#pragma once

#include "glm/glm.hpp"
#include "brdf.hpp"

/*
 * Trowbridge-Reitz aka GGX
 */
class ggx_classic: public brdf
{
public:
  ggx_classic();

  virtual ~ggx_classic()
  {
  }

  virtual void set_alpha(float alpha)
  {
    _alpha = alpha;
  }

  virtual std::string get_name() const
  {
    return "ggx_classic";
  }

  virtual float evaluate(
    const glm::vec3 &light_dir,
    const glm::vec3 &view_dir,
    float &probability_density_function
  ) const;

  virtual glm::vec3 sample(const glm::vec3 &view_dir, const glm::vec2 &random_parameters) const;

protected:
  float normal_ggx(glm::vec3 normal, glm::vec3 half_vector, float a) const;
  float geometry_schlick_ggx(float NdotV, float k) const;
  float geometry_smith(glm::vec3 normal, glm::vec3 view, glm::vec3 light, float k) const;

private:
  float _alpha;
  glm::vec3 _normal;
};
