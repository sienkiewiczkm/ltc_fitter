#include <cstdlib>
#include <iostream>
#include "fitting_settings.hpp"
#include "ltc_lookup_builder.hpp"
#include "ggx.hpp"
#include "ltc.hpp"

#include "ltc_fitting.hpp"
#include "brdf_plot.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#include <boost/math/constants/constants.hpp>
#include "log.hpp"

int main(int argc, const char* argv[])
{
    try
    {
        fitting_settings settings;
        if (!get_fitting_settings_from_command_line(settings, argc, argv))
        {
            log_error() << "failed to set fitting settings, aborting"
                << std::endl;
            return EXIT_FAILURE;
        }

        print_fitting_settings(settings);
        auto result = build_lookup(settings);
    }
    catch (std::exception& exc)
    {
        log_error() << "Exception: " << exc.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (...)
    {
        log_error() << "Unknown exception has been thrown. Aborting."
            << std::endl;

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
