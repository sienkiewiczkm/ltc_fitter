#pragma once
#include "glm/glm.hpp"

bool ray_sphere_min_intersection(
    glm::vec3 origin,
    glm::vec3 direction,
    glm::vec3 sphere_origin,
    float sphere_radius,
    float& intersection_distance
);
