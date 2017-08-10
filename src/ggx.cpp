#include "ggx.hpp"
#include <boost/math/constants/constants.hpp>

ggx::ggx():
    _alpha{0.2f},
    _normal{0.0f, 1.0f, 0.0f}
{
}

float ggx::evaluate(
    const glm::vec3& light_dir,
    const glm::vec3& view_dir,
    float& probability_density_function
) const
{
    if (view_dir.y <= 0)
    {
        probability_density_function = 0.0f;
        return 0.0f;
    }

    glm::vec3 normal{0.0f, 1.0f, 0.0f};
    glm::vec3 half_vector = glm::normalize(view_dir + light_dir);

    float ndf = normal_ggx(normal, half_vector, _alpha);

    float r = std::sqrtf(_alpha) + 1.0f;
    float k = (r*r) / 8.0f;
    float geometry = geometry_smith(normal, view_dir, light_dir, k);

    float fresnel = 1.0f;

    float nominator = ndf * geometry * fresnel;

    float n_dot_l = std::max(glm::dot(normal, light_dir), 0.0f);
    float n_dot_v = std::max(glm::dot(normal, view_dir), 0.0f);
    float denominator = 4.0f * n_dot_l * n_dot_v + 0.001f;

    float n_dot_h = std::max(glm::dot(normal, half_vector), 0.0f);
    float v_dot_h = std::max(glm::dot(view_dir, half_vector), 0.0f);
    probability_density_function = (ndf * n_dot_h) / (4.0f * v_dot_h);

    return nominator / denominator;
}

glm::vec3 ggx::sample(
    const glm::vec3& view_dir,
    const glm::vec2& random_parameters
) const
{
    // based on:
    // - Real Shading in Unreal Engine 4
    //    by Brian Karis, Epic Games
    //   from Siggraph 2013
    // - Notes on Importance Sampling
    //    by Tobias Alexander Franke

    const auto pi = boost::math::constants::pi<float>();

    float phi = 2.0f * pi * random_parameters.x;

    auto cos_theta = std::sqrtf(
        (1 - random_parameters.y) /
            (1 + (_alpha * _alpha - 1) * random_parameters.y)
    );

    auto sin_theta = std::sqrtf(1 - cos_theta * cos_theta);

    glm::vec3 h = {
        sin_theta * std::cos(phi),
        sin_theta * std::sin(phi),
        cos_theta
    };

    auto up_vector = std::fabs(_normal.z) < 0.999f
        ? glm::vec3{0.0f, 0.0f, 1.0f}
        : glm::vec3{1.0f, 0.0f, 0.0f};

    auto tangent_x = glm::normalize(glm::cross(up_vector, _normal));
    auto tangent_y = glm::cross(_normal, tangent_x);

    return tangent_x * h.x + tangent_y * h.y + _normal * h.z;
}

float ggx::pdf(float theta, float phi) const
{
    return 0.0f;
}

float ggx::normal_ggx(glm::vec3 normal, glm::vec3 half_vector, float a) const
{
    float a2 = a * a;
    float n_dot_h = std::max(glm::dot(normal, half_vector), 0.0f);
    float n_dot_h_2 = n_dot_h * n_dot_h;

    const auto pi = boost::math::constants::pi<float>();

    float nominator = a2;
    float denominator = (n_dot_h_2 * (a2 - 1.0f) + 1.0f);
    denominator = pi * denominator * denominator;

    return nominator / denominator;
}

float ggx::geometry_schlick_ggx(float n_dot_v, float k) const
{
    float nominator = n_dot_v;
    float denominator = n_dot_v * (1.0f - k) + k;
    return nominator / denominator;
}

float ggx::geometry_smith(
    glm::vec3 normal,
    glm::vec3 view,
    glm::vec3 light,
    float k
) const
{
    float n_dot_v = std::max(glm::dot(normal, view), 0.0f);
    float n_dot_l = std::max(glm::dot(normal, light), 0.0f);
    float ggx1 = geometry_schlick_ggx(n_dot_l, k);
    float ggx2 = geometry_schlick_ggx(n_dot_v, k);
    return ggx1 * ggx2;
}

