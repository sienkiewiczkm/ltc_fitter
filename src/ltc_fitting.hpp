#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "fitting_settings.hpp"
#include "brdf.hpp"

struct fitting_result
{
    fitting_settings settings;
    std::vector<glm::vec4> transformations;
};

glm::vec4 ltc_fit(brdf& brdf);
glm::vec4 ltc_fit_single(brdf& brdf, glm::vec3 view_dir);

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
