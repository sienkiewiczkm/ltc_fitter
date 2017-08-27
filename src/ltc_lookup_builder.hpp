#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "fitting_settings.hpp"

struct fitting_result
{
    fitting_settings settings;
    std::vector<glm::vec4> transformations;
};

fitting_result build_lookup(const fitting_settings& settings);
