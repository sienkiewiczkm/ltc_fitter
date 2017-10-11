#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "brdf.hpp"
#include "ltc.hpp"

struct ltc_average_terms
{
  ltc_average_terms():
    distribution_norm{},
    fresnel_term{},
    average_direction{}
  {
  }

  ltc_average_terms(
    float distribution_norm,
    float fresnel_term,
    glm::vec3 average_direction
  ):
    distribution_norm{distribution_norm},
    fresnel_term{fresnel_term},
    average_direction{average_direction}
  {
  }

  float distribution_norm;
  float fresnel_term;
  glm::vec3 average_direction;
};

ltc_store_data ltc_fit(brdf &brdf, glm::vec3 view_dir, bool force_isotropic, glm::vec3 &first_guess);
ltc_average_terms calculate_average_terms(const brdf &brdf, const glm::vec3 &view_dir, const int num_samples = 64);
