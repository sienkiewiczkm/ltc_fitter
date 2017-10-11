#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "../numerical/error_estimator.hpp"
#include "../brdf/brdf.hpp"

class ltc_error_estimator: public error_estimator
{
public:
  explicit ltc_error_estimator(const brdf &brdf);
  ~ltc_error_estimator() override = default;

  float estimate_error(const std::vector<float>& parameters) const override;
  float estimate_error(glm::vec3 parameters) const;

  void set_amplitude(float amplitude)
  {
    _amplitude = amplitude;
  }

  void set_view_dir(const glm::vec3 &view_dir)
  {
    _view_dir = view_dir;
  }

  void set_base_frame(const glm::mat3 &base_frame)
  {
    _base_frame = base_frame;
  }

  void set_isotropy_forcing(bool force_isotropy)
  {
    _force_isotropy = force_isotropy;
  }

protected:
  double estimate_partial_error(
    const brdf &sample_source,
    const brdf &other_brdf,
    const glm::vec2 &random_parameters
  ) const;

private:
  const brdf &_brdf;
  float _amplitude;
  glm::vec3 _view_dir;
  glm::mat3 _base_frame;
  bool _force_isotropy;
};
