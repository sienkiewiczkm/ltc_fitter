#include "ltc.hpp"
#include "log.hpp"
#include "boost/math/constants/constants.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

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
    glm::vec3 transformed_light_dir = get_framed_ltc_matrix_inv() * light_dir;
    glm::vec3 original_light_dir = glm::normalize(transformed_light_dir);

    auto l = length(get_framed_ltc_matrix() * original_light_dir);

    const float MIN_TRANSFORMED_LENGTH = 0.0001f;
    if (l < MIN_TRANSFORMED_LENGTH) {
        log_error() << "Transformed light direction to original space "
            << "is degenerated (with length = 0)." << std::endl;

        throw std::logic_error("Transformed light direction is degenerated");
    }

    auto jacobian = glm::determinant(get_framed_ltc_matrix_inv()) / (l*l*l);

    if (std::isnan(jacobian))
    {
        log_error() << "LTC Jacobian is NaN. Aborting." << std::endl;
        log_error() << " Readable information:" << std::endl;
        log_error() << "  view_dir = " << glm::to_string(view_dir) << std::endl;
        log_error() << "  ltc = " << glm::to_string(_ltc_matrix) << std::endl;
        log_error() << "  ltc_inv = " << glm::to_string(_ltc_matrix_inv)
            << std::endl;
        log_error() << "  transformed_view_dir = "
            << glm::to_string(transformed_light_dir) << std::endl;
        log_error() << "  original_view_dir = "
            << glm::to_string(original_light_dir) << std::endl;
        log_error() << " Exact parameters for further debugging: " << std::endl;
        log_error() << "  amplitude = " << std::hexfloat << _amplitude
            << std::endl;
        log_error() << "  light_dir = " << format_exact_vector(light_dir)
            << std::endl;
        log_error() << "  view_dir = " << format_exact_vector(view_dir)
            << std::endl;
        log_error() << "  ltc[0] = " << format_exact_vector(_ltc_matrix[0])
            << std::endl;
        log_error() << "  ltc[1] = " << format_exact_vector(_ltc_matrix[1])
            << std::endl;
        log_error() << "  ltc[2] = " << format_exact_vector(_ltc_matrix[2])
            << std::endl;
        throw std::logic_error("Jacobian is NaN");
    }

    const auto pi = boost::math::constants::pi<float>();
    float d = 1.0f / pi * std::max(0.0f, original_light_dir.z);

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

    return glm::normalize(get_framed_ltc_matrix() * original_sample);
}

void ltc::set_ltc_parameters(const glm::vec4& parameters)
{
    set_ltc_matrix({
        {parameters.x, 0.0f,         parameters.w},
        {0.0f,         parameters.z, 0.0f},
        {parameters.y, 0.0f,         1.0f}
    });
}

void ltc::set_base_frame(const glm::mat3& base_frame)
{
    _base_frame = base_frame;
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

glm::mat3 ltc::get_framed_ltc_matrix() const
{
    return (_base_frame * _ltc_matrix);
}

glm::mat3 ltc::get_framed_ltc_matrix_inv() const
{
    return glm::inverse(get_framed_ltc_matrix());
}

glm::vec4 ltc::get_adjusted_parameters() const
{
    auto matrix = get_framed_ltc_matrix();
    return { matrix[0][0], matrix[2][0], matrix[0][2], matrix[1][1] };
}

void ltc::set_amplitude(float amplitude)
{
    _amplitude = amplitude;
}

float ltc::get_amplitude() const
{
    return _amplitude;
}
