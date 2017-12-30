#define GLM_ENABLE_EXPERIMENTAL

#include <exception>

#include "ltc_fitting.hpp"
#include "boost/math/constants/constants.hpp"
#include "../ltc/ltc_error_estimator.hpp"
#include "glm/gtx/string_cast.hpp"

#include "../utils/log.hpp"
#include "../utils/hacks.hpp"
#include "../numerical/penalty_optimizer.hpp"
#include "../numerical/logarithmic_penalty_error_estimator.hpp"
#include "../numerical/samplers/halton_sampler2d.hpp"

ltc_store_data ltc_fit(brdf &brdf, glm::vec3 view_dir, bool force_isotropic, glm::vec3 &first_guess)
{
  const float SAFE_AMPLITUDE_THRESHOLD = 0.0001f;

  auto average_terms = calculate_average_terms(brdf, view_dir);

  if (average_terms.distribution_norm < SAFE_AMPLITUDE_THRESHOLD)
  {
    log_error() << "BRDF amplitude is too small. It will result in wrong outputs. Aborting." << std::endl;
    log_error() << "View dir" << glm::to_string(view_dir);
    log_error() << "Amplitude = " << average_terms.distribution_norm << std::endl;
    throw std::logic_error("Amplitude is too small.");
  }

  log_debug() << "View direction = " << glm::to_string(view_dir) << std::endl;
  log_debug() << "Avg. direction = " << glm::to_string(average_terms.average_direction) << std::endl;
  log_debug() << "BRDF amplitude = " << average_terms.distribution_norm << std::endl;

  glm::vec3 frame_x{average_terms.average_direction.z, 0.0f, -average_terms.average_direction.x};
  glm::vec3 frame_y{0.0f, 1.0f, 0.0f};
  glm::vec3 frame_z = average_terms.average_direction;
  glm::mat3 frame{frame_x, frame_y, frame_z};

  auto error_estimator = std::make_shared<ltc_error_estimator>(brdf);
  error_estimator->set_amplitude(average_terms.distribution_norm);
  error_estimator->set_view_dir(view_dir);
  error_estimator->set_base_frame(frame);
  error_estimator->set_isotropy_forcing(force_isotropic);

  auto penalty_estimator = std::make_shared<logarithmic_penalty_error_estimator>();
  penalty_estimator->set_base_estimator(error_estimator);

  penalty_optimizer optimizer;
  optimizer.set_penalty_estimator(penalty_estimator);
  auto result_vector = optimizer.optimize({first_guess.x, first_guess.y, first_guess.z});
  glm::vec3 result{result_vector[0], result_vector[1], result_vector[2]};

  log_debug() << "Result: " << glm::to_string(result) << std::endl;

  result = hacks::fix_parameters_below_zero(result);

  if (force_isotropic)
  {
    result.y = result.x;
    result.z = 0.0f;
  }

  log_debug() << "Adjusted result: " << glm::to_string(result) << std::endl;

  ltc ltc;
  ltc.set_amplitude(average_terms.distribution_norm);
  ltc.set_fresnel(average_terms.fresnel_term);
  ltc.set_base_frame(frame);
  ltc.set_ltc_parameters(result);

  return ltc.get_store_data();
}

ltc_average_terms calculate_average_terms(const brdf &brdf, const glm::vec3 &view_dir, const int num_samples)
{
  ltc_average_terms average_terms{};
  halton_sampler2d sampler2d;

  for (auto i = 0; i < num_samples; ++i)
  {
    for (auto j = 0; j < num_samples; ++j)
    {
      const auto random_sample = sampler2d.getNextSample();
      const glm::vec3 light_dir = brdf.sample(view_dir, random_sample);

      float probability_density_function;

      float value = brdf.evaluate(light_dir, view_dir, probability_density_function);
      float weight = probability_density_function > 0.0f ? value / probability_density_function : 0.0f;

      average_terms.distribution_norm += weight;

      if (std::isnan(average_terms.distribution_norm))
      {
        log_error() << "Distribution norm is NaN." << std::endl;
        throw std::logic_error("Distribution norm is NaN.");
      }

      // You can find explanation of this calculation there:
      //   LTC Fresnel Approximation by Stephen Hill
      //   http://blog.selfshadow.com/publications/s2016-advances/s2016_ltc_fresnel.pdf
      //
      // In short, we need to store:
      //   int {1-dot(v,h)}^{5} * brdf(v,l) * cos(theta) dl
      // which will be labeled as 'fresnel'.
      auto half_vec = glm::normalize(light_dir + view_dir);
      auto fresnel_part = pow(1.0f - glm::max(glm::dot(view_dir, half_vec), 0.0f), 5.0f);

      average_terms.fresnel_term += weight * fresnel_part;
      average_terms.average_direction += weight * light_dir;
    }
  }

  average_terms.distribution_norm /= static_cast<float>(num_samples * num_samples);
  average_terms.fresnel_term /= static_cast<float>(num_samples * num_samples );

  // Limit average direction to XZ plane
  average_terms.average_direction.y = 0.0f;
  average_terms.average_direction = glm::normalize(average_terms.average_direction);

  return average_terms;
}
