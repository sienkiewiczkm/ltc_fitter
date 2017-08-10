#include "ltc_fitting.hpp"
#include "boost/math/constants/constants.hpp"
#include "ggx.hpp"
#include "ltc_nelder_mead.hpp"

glm::vec4 ltc_fit(brdf& brdf)
{
    const auto pi = boost::math::constants::pi<float>();
    const float min_alpha = 0.0001f;
    const int resolution = 256;

    for (auto i = 0; i < resolution; ++i)
    {
        for (auto j = 0; j < resolution; ++j)
        {
            const float theta = std::min(
                pi/2.0f - 0.08f,
                j/static_cast<float>(resolution-1)*pi/2.0f
            );

            const glm::vec3 view_dir{
                std::sinf(theta),
                0.0f,
                std::cosf(theta)
            };
        }
    }

    return {};
}

glm::vec4 ltc_fit_single(brdf& brdf, glm::vec3 view_dir)
{
    float amplitude = compute_distribution_norm(brdf, view_dir);

    glm::vec4 first_guess{1.0f, 0.0f, 1.0f, 0.0f};

    ltc_nelder_mead optimizer{brdf};
    optimizer.set_amplitude(amplitude);
    optimizer.set_view_dir(view_dir);
    auto result = optimizer.optimize(first_guess);

    return result;
}

float compute_distribution_norm(
    const brdf& brdf,
    const glm::vec3& view_dir,
    const int num_samples
)
{
    float distribution_norm = 0.0f;

    for (auto i = 0; i < num_samples; ++i)
    {
        for (auto j = 0; j < num_samples; ++j)
        {
            const float u = (i+0.5f)/static_cast<float>(num_samples);
            const float v = (j+0.5f)/static_cast<float>(num_samples);

            const glm::vec3 light_dir = brdf.sample(view_dir, {u, v});

            float probability_density_function;
            float value = brdf.evaluate(
                light_dir,
                view_dir,
                probability_density_function
            );

            distribution_norm += (probability_density_function > 0.0f)
                ? value / probability_density_function
                : 0.0f;
        }
    }

    return distribution_norm / static_cast<float>(num_samples*num_samples);
}

glm::vec3 compute_average_direction(
    const brdf& brdf,
    const glm::vec3& view_dir,
    const int num_samples
)
{
    glm::vec3 average_direction{};

    for (auto i = 0; i < num_samples; ++i)
    {
        for (auto j = 0; j < num_samples; ++j)
        {
            const float u = (i+0.5f)/static_cast<float>(num_samples);
            const float v = (j+0.5f)/static_cast<float>(num_samples);
            const glm::vec3 light_dir = brdf.sample(view_dir, {u, v});

            float probability_density_function;
            float value = brdf.evaluate(
                light_dir,
                view_dir,
                probability_density_function
            );

            average_direction += (probability_density_function > 0.0f)
                ? value / probability_density_function * light_dir
                : glm::vec3{};
        }
    }

    average_direction.z = 0.0f;

    return glm::normalize(average_direction);
}
