#include "ltc_fitting.hpp"
#include "boost/math/constants/constants.hpp"
#include "ggx.hpp"
#include "ltc_nelder_mead.hpp"

#include "log.hpp"

glm::vec4 ltc_fit(
    brdf& brdf,
    glm::vec3 view_dir,
    glm::vec4 initial_parameters,
    glm::mat3 initial_frame
)
{
    const float SAFE_AMPLITUDE_THRESHOLD = 0.0001f;

    float amplitude = compute_distribution_norm(brdf, view_dir);

    if (amplitude < SAFE_AMPLITUDE_THRESHOLD)
    {
        log_error() << "Amplitude is too small. "
            << "It will result in wrong outputs. Aborting."
            << std::endl;

        throw std::logic_error("Amplitude is too small.");
    }

    ltc_nelder_mead optimizer{brdf};
    optimizer.set_amplitude(amplitude);
    optimizer.set_view_dir(view_dir);
    auto result = optimizer.optimize(initial_parameters);

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
