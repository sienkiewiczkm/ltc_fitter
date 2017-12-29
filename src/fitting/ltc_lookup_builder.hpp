#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "../fitting_settings.hpp"
#include "../ltc/ltc.hpp"

struct fitting_result
{
  fitting_settings settings;
  std::vector<ltc_store_data> transformations;
};

fitting_result build_lookup(const fitting_settings &settings);
