#include "ltc_fitting.hpp"

fitting_result ltc_fit(fitting_settings settings)
{
    fitting_result result;
    result.settings = settings;

    for (auto i = 0; i < settings.resolution; ++i)
    {
        for (auto j = 0; j < settings.resolution; ++j)
        {
            result.transformations.push_back(glm::vec4{1.0f, 0.0f, 0.0f, 1.0f});
        }
    }

    return result;
}
