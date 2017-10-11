#pragma once

#include "glm/glm.hpp"

class vector_utils
{
public:
  vector_utils() = default;
  ~vector_utils() = default;

  static std::vector<float> add(const std::vector<float>& a, const std::vector<float>& b);
  static std::vector<float> sub(const std::vector<float>& a, const std::vector<float>& b);
  static std::vector<float> mul(const std::vector<float>& input, float multipler);
  static std::vector<float> div(const std::vector<float>& input, float divisor);
  static float sum(const std::vector<float>& a);
  static std::vector<float> abs(const std::vector<float>& input);
};
