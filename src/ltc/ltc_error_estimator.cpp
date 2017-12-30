#include "ltc_error_estimator.hpp"
#include "ltc.hpp"
#include "../utils/hacks.hpp"
#include "../numerical/samplers/halton_sampler2d.hpp"

ltc_error_estimator::ltc_error_estimator(
  const brdf &brdf
):
  _brdf{brdf},
  _amplitude{1.0f}
{
}

float ltc_error_estimator::estimate_error(const std::vector<float>& parameters) const
{
  glm::vec3 params{parameters[0], parameters[1], parameters[2]};
  return estimate_error(params);
}

float ltc_error_estimator::estimate_error(glm::vec3 parameters) const
{
  const int num_samples = 32;

  if (_force_isotropy)
  {
    parameters.y = parameters.x;
    parameters.z = 0.0f;
  }

  //parameters = hacks::fix_parameters_below_zero(parameters);

  double total_error = 0.0f;

  ltc ltc;
  ltc.set_amplitude(_amplitude);
  ltc.set_base_frame(_base_frame);
  ltc.set_ltc_parameters(parameters);

  halton_sampler2d sampler2d;
  for (auto i = 0; i < num_samples; ++i)
  {
    for (auto j = 0; j < num_samples; ++j)
    {
      const auto random_sample = sampler2d.getNextSample();

      total_error += estimate_partial_error(ltc, _brdf, random_sample);
      total_error += estimate_partial_error(_brdf, ltc, random_sample);
    }
  }

  float final_error = static_cast<float>(total_error / static_cast<double>(num_samples * num_samples));
  return final_error;
}

double ltc_error_estimator::estimate_partial_error(
  const brdf &sample_source,
  const brdf &other_brdf,
  const glm::vec2 &random_parameters
) const
{
  const glm::vec3 light_dir = sample_source.sample(_view_dir, random_parameters);

  float source_pdf, other_pdf;

  auto source_value = sample_source.evaluate(light_dir, _view_dir, source_pdf);
  auto other_value = other_brdf.evaluate(light_dir, _view_dir, other_pdf);

  source_pdf = fabs(source_pdf);
  other_pdf = fabs(other_pdf);

  // TODO: Figure out why do we sum both PDFs
  double error1 = std::fabs(source_value - other_value);
  auto error3 = error1 * error1 * error1;
  auto partial_error = error3 / (source_pdf + other_pdf);

  return partial_error;
}
