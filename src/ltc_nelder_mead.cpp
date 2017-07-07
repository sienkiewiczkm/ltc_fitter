#include "ltc_nelder_mead.hpp"

#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

ltc_nelder_mead::ltc_nelder_mead()
{
}

float ltc_nelder_mead::estimate_error(glm::vec4 parameters)
{
    return std::fabs(parameters.x) + std::fabs(parameters.y)
        + std::fabs(parameters.z) + std::fabs(parameters.w);
}


