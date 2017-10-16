#include "ltc_lookup_builder.hpp"
#include "../brdf/brdf.hpp"
#include "../brdf/ggx.hpp"
#include "../ltc/ltc.hpp"
#include "ltc_fitting.hpp"
#include "../utils/helpers.hpp"
#include "../utils/log.hpp"
#include "../plotting/brdf_plot.hpp"
#include "glm/glm.hpp"
#include "boost/math/constants/constants.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

#define GLM_ENABLE_EXPERIMENTAL

#include "glm/gtx/string_cast.hpp"
#include "../brdf/ggx_slope.hpp"

fitting_result build_lookup(const fitting_settings &settings)
{
  brdf *brdf;

  if (settings.brdf_method == "ggx")
  {
    // TODO: use real ggx
    brdf = new ggx_slope;
  }
  else
  {
    throw std::invalid_argument("Provided BRDF method is unsupported.");
  }

  log_info() << "Fitting process started." << std::endl;
  log_info() << "  % Angle Roughness Parameters" << std::endl;

  fitting_result result;
  result.settings = settings;

  for (auto rough_frag = 0; rough_frag < settings.resolution; ++rough_frag)
  {
    const auto rough_perc = rough_frag / static_cast<float>(settings.resolution - 1);

    // TODO: Restore support for roughness boundaries
    const auto roughness = rough_perc;

    const float MIN_ALPHA = 0.0001f;
    auto alpha = roughness * roughness;
    brdf->set_alpha(std::max(MIN_ALPHA, alpha));

    glm::vec3 last_result{0.4f, 0.4f, 0.0f};

    for (auto angle_frag = 0; angle_frag < settings.resolution; ++angle_frag)
    {
      // Parametrize to be equally distributed along the axis, not sphere.
      // This can be achieved by using parametrisation of upper part of
      // the circle. Y = sqrt(R^2 - X^2) and calculating cosine there.
      // Also, limit the angle to 1.57 which is a bit below PI/2,
      // we do not want to calculate for orthogonal case.
      const auto angle_perc = angle_frag / static_cast<float>(settings.resolution - 1);
      float cos_theta = 1.0f - angle_perc * angle_perc;
      float theta = std::min(1.57f, std::acosf(cos_theta));
      glm::vec3 view_dir{std::sinf(theta), 0.0f, std::cosf(theta)};

      auto work_percent = (rough_frag * settings.resolution + angle_frag)
        / static_cast<float>(settings.resolution * settings.resolution);

      log_info() << std::setw(3) << static_cast<int>(work_percent * 100) << " " << std::setprecision(3)
        << std::setw(5) << theta << " " << std::setw(9) << roughness << std::endl;

      ltc_store_data data;
      glm::vec3 first_guess = last_result;

      try
      {
        data = ltc_fit(*brdf, view_dir, angle_frag == 0, first_guess);
      }
      catch (...)
      {
      }

      std::stringstream ss;
      ss << "fit_r" << rough_frag << "_a" << angle_frag << "_";

      ltc ltc;
      ltc.set_store_data(data);

      brdf_plot plotter;
      plotter.set_view_dir(view_dir);
      plotter.set_resolution(256);
      plotter.export_png(brdf, ss.str() + "brdf.png");
      plotter.export_png(&ltc, ss.str() + "ltc.png");

      //result.transformations.push_back(parameters);
    }
  }

  log_info() << "Fitting process completed." << std::endl;

  return result;
}
