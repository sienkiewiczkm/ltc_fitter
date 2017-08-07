#pragma once
#include "glm/glm.hpp"

class brdf
{
public:
    virtual ~brdf() {}

    virtual float evaluate(
        const glm::vec3& light_dir,
        const glm::vec3& view_dir,
        float& probability_density_function
    ) = 0;

    virtual glm::vec3 sample(
        const glm::vec3& view_dir,
        const glm::vec2& random_parameters
    ) = 0;
};
