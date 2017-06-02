#include <cstdlib>
#include <iostream>
#include "fitting_settings.hpp"
#include "ltc_fitting.hpp"
#include "result_saving.hpp"

int main(int argc, const char* argv[])
{
    fitting_settings settings;
    if (!get_fitting_settings_from_command_line(settings, argc, argv))
    {
        std::cout << "failed to set fitting settings" << std::endl;
        return EXIT_FAILURE;
    }

    print_fitting_settings(settings);
    auto result = ltc_fit(settings);
    save_fitting_result(result);

    return EXIT_SUCCESS;
}
