#include "logarithmic_penalty_error_estimator.hpp"
#include <cmath>

float logarithmic_penalty_error_estimator::estimate_error(const std::vector<float>& parameters) const
{
  auto base_estimation = _base_estimator->estimate_error(parameters);
  auto penalty = calculate_penalty(parameters);
  return base_estimation + penalty;
}

float logarithmic_penalty_error_estimator::calculate_penalty(std::vector<float> parameters) const
{
  if (_constraints_calculator == nullptr)
  {
    return 0.0f;
  }

  float accumulator = 0.0f;
  auto constraints_values = _constraints_calculator->calculate_constraints(parameters);

  for (auto i = 0; i < constraints_values.size(); ++i)
  {
    auto single_penalty = std::log(-constraints_values[i]);
    accumulator += single_penalty;
  }

  return -_penalty_factor * accumulator;
}


