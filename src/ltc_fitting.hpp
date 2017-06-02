#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "fitting_settings.hpp"

struct fitting_result
{
    fitting_settings settings;
    std::vector<glm::vec4> transformations;
};

fitting_result ltc_fit(fitting_settings settings);
