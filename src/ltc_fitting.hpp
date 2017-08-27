#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "brdf.hpp"

glm::vec4 ltc_fit(
    brdf& brdf,
    glm::vec3 view_dir,
    glm::vec4 initial_parameters,
    glm::mat3 initial_frame
);

float compute_distribution_norm(
    const brdf& brdf,
    const glm::vec3& view_dir,
    const int num_samples = 64
);

glm::vec3 compute_average_direction(
    const brdf& brdf,
    const glm::vec3& view_dir,
    const int num_samples = 64
);
