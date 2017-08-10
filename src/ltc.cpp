#include "ltc.hpp"
#include "boost/math/constants/constants.hpp"

ltc::ltc():
    _amplitude{1.0f},
    _ltc_matrix{},
    _ltc_matrix_inv{},
    _ltc_matrix_inv_determinant{1.0f}
{
}

float ltc::evaluate(
    const glm::vec3& light_dir,
    const glm::vec3& view_dir,
    float& probability_density_function
) const
{
    // here 'original' means original clamped cosine distrubution 'space'
    glm::vec3 original_light_dir = glm::normalize(_ltc_matrix_inv * light_dir);

    auto l = length(_ltc_matrix_inv * light_dir);
    auto jacobian = _ltc_matrix_inv_determinant / (l*l*l);

    const auto pi = boost::math::constants::pi<float>();
    float d = 1.0f / pi * std::max(0.0f, original_light_dir.y);

    float result = _amplitude * d * jacobian;

    probability_density_function = result / _amplitude;

    return result;
}

glm::vec3 ltc::sample(
    const glm::vec3& view_dir,
    const glm::vec2& random_parameters
) const
{
    const auto pi = boost::math::constants::pi<float>();
    const float theta = std::acosf(std::sqrtf(random_parameters.x));
    const float phi = 2.0f * pi * random_parameters.y;

    const glm::vec3 original_sample{
        std::sinf(theta) * std::cosf(phi),
        std::sinf(theta) * std::sinf(phi),
        std::cosf(theta)
    };

    return glm::normalize(_ltc_matrix * original_sample);
}

void ltc::set_ltc_matrix(const glm::mat3& mat)
{
    _ltc_matrix = mat;
    _ltc_matrix_inv = glm::inverse(_ltc_matrix);
    _ltc_matrix_inv_determinant = glm::determinant(_ltc_matrix_inv);
}

const glm::mat3& ltc::get_ltc_matrix() const
{
    return _ltc_matrix;
}

const glm::mat3& ltc::get_ltc_matrix_inv() const
{
    return _ltc_matrix_inv;
}

void ltc::set_amplitude(float amplitude)
{
    _amplitude = amplitude;
}

float ltc::get_amplitude() const
{
    return _amplitude;
}
