#define GLM_ENABLE_EXPERIMENTAL

#include "ltc_fitting.hpp"
#include "boost/math/constants/constants.hpp"
#include "ggx.hpp"
#include "ltc_nelder_mead.hpp"
#include "ltc.hpp"
#include "glm/gtx/string_cast.hpp"

#include "log.hpp"

glm::vec4 ltc_fit(brdf& brdf, glm::vec3 view_dir)
{
    const float SAFE_AMPLITUDE_THRESHOLD = 0.0001f;

    auto average_terms = calculate_average_terms(brdf, view_dir);

    if (average_terms.distribution_norm < SAFE_AMPLITUDE_THRESHOLD)
    {
        log_error() << "BRDF amplitude is too small. "
            << "It will result in wrong outputs. Aborting."
            << std::endl;

        log_error() << "View dir" << glm::to_string(view_dir);
        log_error() << "Amplitude = " << average_terms.distribution_norm
            << std::endl;

        throw std::logic_error("Amplitude is too small.");
    }

    log_debug() << "View direction = " << glm::to_string(view_dir) << std::endl;
    log_debug() << "BRDF amplitude = " << average_terms.distribution_norm
        << std::endl;

    glm::vec3 frame_x{
        average_terms.average_direction.z,
        0.0f,
        -average_terms.average_direction.x
    };
    glm::vec3 frame_y{0.0f, 1.0f, 0.0f};
    glm::vec3 frame_z = average_terms.average_direction;
    glm::mat3 frame{frame_x, frame_y, frame_z};

    glm::vec4 first_guess{1.0f, 0.0f, 1.0f, 0.0f};

    ltc_nelder_mead optimizer{brdf};
    optimizer.set_amplitude(average_terms.distribution_norm);
    optimizer.set_view_dir(view_dir);
    optimizer.set_base_frame(frame);
    auto result = optimizer.optimize(first_guess);

    ltc ltc;
    ltc.set_base_frame(frame);
    ltc.set_ltc_parameters(result);
    return ltc.get_adjusted_parameters();
}

ltc_average_terms calculate_average_terms(
    const brdf& brdf,
    const glm::vec3 &view_dir,
    const int num_samples
)
{
    ltc_average_terms average_terms{};

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

            float weight = value / probability_density_function;

            // PDF should be always positive!
            // This workaround should be (hopefully) redundant once all bugs
            // will be found and fixed.
            // TODO: Remove when time comes.
            if (probability_density_function < 0.0f)
            {
                weight = 0.0f;
            }

            average_terms.distribution_norm += weight;

            // You can find explanation of this calculation there:
            //   LTC Fresnel Approximation by Stephen Hill
            //   http://blog.selfshadow.com/publications/s2016-advances/s2016_ltc_fresnel.pdf
            //
            // In short, we need to store:
            //   int {1-dot(v,h)}^{5} * brdf(v,l) * cos(theta) dl
            // which will be labeled as 'fresnel'.

            auto half_vec = glm::normalize(light_dir + view_dir);
            auto fresnel_part = pow(
                1.0f - glm::max(glm::dot(view_dir, half_vec), 0.0f),
                5.0f
            );

            average_terms.fresnel_term += weight * fresnel_part;

            average_terms.average_direction += weight * light_dir;
        }
    }

    // Limit average direction to XZ plane
    average_terms.average_direction.y = 0.0f;
    average_terms.average_direction = glm::normalize(
        average_terms.average_direction
    );

    average_terms.distribution_norm /= static_cast<float>(
        num_samples * num_samples
    );

    average_terms.fresnel_term /= static_cast<float>(
        num_samples * num_samples
    );

    return average_terms;
}
