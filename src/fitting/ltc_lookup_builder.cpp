#include "ltc_lookup_builder.hpp"
#include "../brdf/brdf.hpp"
#include "../ltc/ltc.hpp"
#include "ltc_fitting.hpp"
#include "../utils/helpers.hpp"
#include "../utils/log.hpp"
#include "../plotting/brdf_plot.hpp"
#include "glm/glm.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <algorithm>

#define GLM_ENABLE_EXPERIMENTAL

#include "glm/gtx/string_cast.hpp"
#include "../brdf/ggx.hpp"
#include "fitting_result.hpp"

fitting_result build_lookup(const fitting_settings &settings)
{
  brdf *brdf;

  if (settings.brdf_method == "ggx")
  {
    brdf = new ggx;
  }
  else
  {
    throw std::invalid_argument("Requested BRDF method is not supported.");
  }

  log_info() << "Fitting process started." << std::endl;
  log_info() << "  % Angle Roughness Parameters" << std::endl;

  fitting_result result;
  result.resize(settings.resolution, settings.resolution);

  glm::vec3 next_initial_parameters{1.0f, 1.0f, 0.0f};

  int numFitted = 0;

  for (auto rough_frag = settings.resolution - 1; rough_frag >= 0; --rough_frag)
  {
    const auto rough_perc = rough_frag / static_cast<float>(settings.resolution - 1);

    // TODO: Restore support for roughness boundaries
    const auto roughness = rough_perc;

    const float MIN_ALPHA = 0.0001f;
    auto alpha = std::max(roughness * roughness, MIN_ALPHA);
    brdf->set_alpha(alpha);

    // This formula was created by hand to approximate (very roughly) initial roughness to speed up the process a bit.
    //const float first_scale_guess = 0.01f + 0.45f * roughness + 0.8f * roughness * roughness;
    //glm::vec3 last_result{first_scale_guess, first_scale_guess, 0.0f};

	glm::vec3 last_result = next_initial_parameters;

    for (auto angle_frag = 0; angle_frag < settings.resolution; ++angle_frag)
    {
      // Parametrize to be equally distributed along the axis, not sphere.
      // This can be achieved by using parametrisation of upper part of
      // the circle. Y = sqrt(R^2 - X^2) and calculating cosine there.
      // Also, limit the angle to 1.57 which is a bit below PI/2,
      // we do not want to calculate for orthogonal case.
      //
      //  |        /
      //  |theta /
      //  |    /  ||V||=1
      //  |  /
      //  |/
      //
      const auto angle_perc = angle_frag / static_cast<float>(settings.resolution - 1);
      float cos_theta = 1.0f - angle_perc * angle_perc;
      float theta = std::min(1.57f, std::acos(cos_theta));
      glm::vec3 view_dir{std::sin(theta), 0.0f, std::cos(theta)};

      auto work_percent = numFitted / static_cast<float>(settings.resolution * settings.resolution);

      log_info() << std::setw(3) << static_cast<int>(work_percent * 100) << " " << std::setprecision(3)
        << std::setw(5) << theta << " " << std::setw(9) << roughness << std::endl;

      ltc_store_data data;

      // Use the last result, but throw skewness factor away
      glm::vec3 first_guess = last_result;
      first_guess.z = 0.0f;

      log_info() << "First guess: " << glm::to_string(first_guess) << std::endl;

      try
      {
        glm::vec3 next_first_guess;
        data = ltc_fit(*brdf, view_dir, angle_frag == 0, first_guess, next_first_guess);
        last_result = next_first_guess;
		if (angle_frag == 0) {
			next_initial_parameters = next_first_guess;
		}
        log_info() << "Result (next guess): " << glm::to_string(next_first_guess) << std::endl;
      }
      catch (...)
      {
		log_info() << "Exception was thrown. Aborting fitting process for current configuration.";
      }

      // TODO: Add option for image previews generation on the way.
      {
        ltc ltc;
        ltc.set_store_data(data);

        brdf_plot plotter;
        plotter.set_view_dir(view_dir);
        plotter.set_resolution(256);

        std::stringstream ss;
        ss << "fit_r" << rough_frag << "_a" << angle_frag << "_";

        plotter.export_png(brdf, ss.str() + "brdf.png");
        plotter.export_png(&ltc, ss.str() + "ltc.png");
      }

      result.setData(angle_frag, rough_frag, data);
	  ++numFitted;
    }
  }

  log_info() << "Fitting process completed." << std::endl;

  return result;
}
