#pragma once

#include <vector>

class error_estimator
{
public:
  virtual ~error_estimator() = default;
  virtual float estimate_error(const std::vector<float>& parameters) const = 0;
};
