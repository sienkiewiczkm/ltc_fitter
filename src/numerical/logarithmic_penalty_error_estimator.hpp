#pragma once

#include <memory>
#include "error_estimator.hpp"
#include "penalty_error_estimator.hpp"

class logarithmic_penalty_error_estimator: public penalty_error_estimator
{
public:
  logarithmic_penalty_error_estimator() = default;
  ~logarithmic_penalty_error_estimator() override = default;

  void set_penalty_factor(float value) override
  {
    _penalty_factor = value;
  }

  void set_base_estimator(std::shared_ptr<error_estimator> estimator) override
  {
    _base_estimator = estimator;
  }

  void set_constraints(std::shared_ptr<constraints_calculator> constraints_calculator) override
  {
    _constraints_calculator = constraints_calculator;
  }

  float estimate_error(const std::vector<float>& parameters) const override;

protected:
  float calculate_penalty(std::vector<float> parameters) const;

private:
  float _penalty_factor;
  std::shared_ptr<error_estimator> _base_estimator;
  std::shared_ptr<constraints_calculator> _constraints_calculator;
};
