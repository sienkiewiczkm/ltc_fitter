#include "ltc_nelder_mead.hpp"
#include "ltc.hpp"
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "log.hpp"

ltc_nelder_mead::ltc_nelder_mead(
    const brdf& brdf
):
    _brdf{brdf},
    _amplitude{1.0f}
{
}

float ltc_nelder_mead::estimate_error(glm::vec4 parameters)
{
    const int num_samples = 16;

    float total_error = 0.0f;

    ltc ltc;
    ltc.set_amplitude(_amplitude);
    ltc.set_ltc_matrix({
        {parameters.x, parameters.y, 0.0f},
        {parameters.w, 1.0f, 0.0f},
        {0.0f, 0.0f, parameters.z}
    });

    for (auto i = 0; i < num_samples; ++i)
    {
        for (auto j = 0; j < num_samples; ++j)
        {
            const float u = (i+0.5f)/static_cast<float>(num_samples);
            const float v = (j+0.5f)/static_cast<float>(num_samples);

            total_error += estimate_partial_error(ltc, _brdf, {u, v});
            total_error += estimate_partial_error(_brdf, ltc, {u, v});
        }
    }

    return total_error/static_cast<double>(num_samples*num_samples);
}

double ltc_nelder_mead::estimate_partial_error(
    const brdf& sample_source,
    const brdf& other_brdf,
    const glm::vec2& random_parameters
)
{
    const glm::vec3 light_dir = sample_source.sample({}, random_parameters);

    float source_pdf, other_pdf;

    auto source_value = sample_source.evaluate(
        light_dir,
        _view_dir,
        source_pdf
    );

    auto other_value = other_brdf.evaluate(
        light_dir,
        _view_dir,
        other_pdf
    );

    double error1 = std::fabs(source_value - other_value);
    auto error3 = error1 * error1 * error1;
    auto partial_error = error3 / (source_pdf + other_pdf + 0.001f);

    return partial_error;
}
