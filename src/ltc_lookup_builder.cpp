#include "ltc_lookup_builder.hpp"
#include "brdf.hpp"
#include "ggx.hpp"
#include "ltc_fitting.hpp"
#include "helpers.hpp"
#include "log.hpp"
#include "glm/glm.hpp"
#include "boost/math/constants/constants.hpp"
#include <iostream>
#include <iomanip>

fitting_result build_lookup(const fitting_settings& settings)
{
    brdf *brdf;

    if (settings.brdf_method == "ggx")
    {
        brdf = new ggx;
    }
    else
    {
        throw std::invalid_argument("Provided BRDF method is unsupported.");
    }

    log_info() << "Fitting process started." << std::endl;
    log_info() << "  % Angle Roughness" << std::endl;

    fitting_result result;
    result.settings = settings;

    for (auto rough_frag = 0; rough_frag < settings.resolution; ++rough_frag)
    for (auto angle_frag = 0; angle_frag < settings.resolution; ++angle_frag)
    {
        auto rough_perc = rough_frag/static_cast<float>(settings.resolution-1);
        auto angle_perc = angle_frag/static_cast<float>(settings.resolution-1);

        auto roughness = glm::mix(
            settings.min_roughness,
            settings.max_roughness,
            rough_perc
        );

        auto alpha = roughness * roughness;
        brdf->set_alpha(alpha);

        const auto pi = boost::math::constants::pi<float>();
        auto angle = glm::mix(0.005f, pi/4.0f, angle_perc);

        glm::vec3 view_dir{std::sinf(angle), std::cosf(angle), 0.0f};

        auto work_percent = (rough_frag*settings.resolution + angle_frag)
            /static_cast<float>(settings.resolution*settings.resolution);

        log_info()
            << std::setw(3) << static_cast<int>(work_percent*100) << " "
            << std::setprecision(3) << std::setw(5) << angle << " "
            << std::setw(9) << roughness
            << std::endl;


        glm::mat3 initial_frame{1.0f};
        glm::vec4 initial_parameters{};

        // generate first guess
        if (angle_frag == 0)
        {
            if (rough_frag == 0)
            {
                initial_parameters = {1.0, 0.0f, 1.0f, 0.0f};
            }
            else
            {
                auto index = (rough_frag - 1) * settings.resolution;
                auto similar_parameters = result.transformations[index];
                initial_parameters = similar_parameters;
            }
        }
        else
        {
            auto average_direction = glm::normalize(
                compute_average_direction(*brdf, view_dir)
            );

            auto x_axis = glm::vec3{
                average_direction.z,
                0.0f,
                -average_direction.x
            };

            auto y_axis = glm::vec3{0.0f, 1.0f, 0.0f};
            auto z_axis = average_direction;

            initial_frame = {x_axis, y_axis, z_axis};

            auto last_parameters = result.transformations.back();
            initial_parameters = last_parameters;
        }

        glm::vec4 result_parameters{};

        try {
            result_parameters = ltc_fit(
                *brdf,
                view_dir,
                initial_parameters,
                initial_frame
            );
        } catch (...) {
        }

        result.transformations.push_back(result_parameters);
    }

    log_info() << "Fitting process completed." << std::endl;

    return result;
}
