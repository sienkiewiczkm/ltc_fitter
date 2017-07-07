#include <cstdlib>
#include <iostream>
#include "fitting_settings.hpp"
#include "ltc_fitting.hpp"
#include "result_saving.hpp"
#include "ltc_nelder_mead.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

int main(int argc, const char* argv[])
{
    ltc_nelder_mead nm;
    auto opt_result = nm.optimize({1.0f, 2.0f, 3.0f, 4.0f});
    std::cout << "result : " << glm::to_string(opt_result) << std::endl;

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
