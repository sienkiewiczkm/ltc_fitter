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

        glm::vec3 view_dir{std::cosf(angle), 0.0f, std::sinf(angle)};

        auto work_percent = (rough_frag*settings.resolution + angle_frag)
            /static_cast<float>(settings.resolution*settings.resolution);

        log_info()
            << std::setw(3) << static_cast<int>(work_percent*100) << " "
            << std::setprecision(3) << std::setw(5) << angle << " "
            << std::setw(9) << roughness
            << std::endl;

        glm::vec4 parameters{};

        try {
            parameters = ltc_fit(*brdf, view_dir);
        } catch (...) {
        }

        result.transformations.push_back(parameters);
    }

    log_info() << "Fitting process completed." << std::endl;

    return result;
}
