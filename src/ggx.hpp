#pragma once
#include "glm/glm.hpp"
#include "brdf.hpp"

/*
 * Trowbridge-Reitz aka GGX
 */
class ggx: public brdf
{
public:
    ggx();
    virtual ~ggx() {}

    void setAlpha(float alpha) { _alpha = alpha; }

    virtual float evaluate(
        const glm::vec3& light_dir,
        const glm::vec3& view_dir,
        float& probability_density_function
    ) const;

    virtual glm::vec3 sample(
        const glm::vec3& view_dir,
        const glm::vec2& random_parameters
    ) const;

    float pdf(float theta, float phi) const;

protected:
    float normal_ggx(glm::vec3 normal, glm::vec3 half_vector, float a) const;
    float geometry_schlick_ggx(float NdotV, float k) const;
    float geometry_smith(
        glm::vec3 normal,
        glm::vec3 view,
        glm::vec3 light,
        float k
    ) const;

private:
    float _alpha;
    glm::vec3 _normal;
};
