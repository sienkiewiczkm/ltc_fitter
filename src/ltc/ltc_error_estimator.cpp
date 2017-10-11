#include "ltc_error_estimator.hpp"
#include "../ltc.hpp"
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL

#include "glm/gtx/string_cast.hpp"
#include "../log.hpp"

ltc_error_estimator::ltc_error_estimator(
  const brdf &brdf
):
  _brdf{brdf},
  _amplitude{1.0f}
{
}

/*
glm::vec3 ltc_error_estimator::optimize(glm::vec3 start_parameters)
{
  std::vector<float> input{start_parameters.x, start_parameters.y, start_parameters.z};

  auto result = nelder_mead::optimize(input);

  if (result.size() != 3)
  {
    throw std::logic_error("Returned result has unexpected dimmension");
  }

  return {result[0], result[1], result[2]};
}
*/

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

  parameters = glm::max(parameters, {0.05f, 0.05f, 0.0f});

  float total_error = 0.0f;

  ltc ltc;
  ltc.set_amplitude(_amplitude);
  ltc.set_ltc_parameters(parameters);
  ltc.set_base_frame(_base_frame);

  for (auto i = 0; i < num_samples; ++i)
  {
    for (auto j = 0; j < num_samples; ++j)
    {
      const float u = (i + 0.5f) / static_cast<float>(num_samples);
      const float v = (j + 0.5f) / static_cast<float>(num_samples);

      total_error += estimate_partial_error(ltc, _brdf, {u, v});
      total_error += estimate_partial_error(_brdf, ltc, {u, v});
    }
  }

  auto final_error = total_error / static_cast<double>(num_samples * num_samples);
  return final_error;
}

double ltc_error_estimator::estimate_partial_error(
  const brdf &sample_source,
  const brdf &other_brdf,
  const glm::vec2 &random_parameters
) const
{
  const glm::vec3 light_dir = sample_source.sample({}, random_parameters);

  float source_pdf, other_pdf;

  auto source_value = sample_source.evaluate(light_dir, _view_dir, source_pdf);
  auto other_value = other_brdf.evaluate(light_dir, _view_dir, other_pdf);

  source_pdf = fabs(source_pdf);
  other_pdf = fabs(other_pdf);

  double error1 = std::fabs(source_value - other_value);
  auto error3 = error1 * error1 * error1;
  auto partial_error = error3 / (source_pdf + other_pdf + 0.001f);

  return partial_error;
}
