#pragma once

#include <memory>
#include "error_estimator.hpp"
#include "constraints_calculator.hpp"

class penalty_error_estimator: public error_estimator
{
public:
  penalty_error_estimator() = default;
  ~penalty_error_estimator() override = default;

  virtual void set_penalty_factor(float value) = 0;
  virtual void set_base_estimator(std::shared_ptr<error_estimator> estimator) = 0;
  virtual void set_constraints(std::shared_ptr<constraints_calculator> constraints_calculator) = 0;
};
