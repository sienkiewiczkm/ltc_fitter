#pragma once

#include "penalty_error_estimator.hpp"

class penalty_optimizer
{
public:
  penalty_optimizer() = default;
  ~penalty_optimizer() = default;

  void set_penalty_estimator(std::shared_ptr<penalty_error_estimator> estimator)
  {
    _estimator = estimator;
  }

  std::vector<float> optimize(const std::vector<float>& start_parameters) const;

private:
  std::shared_ptr<penalty_error_estimator> _estimator;
};
