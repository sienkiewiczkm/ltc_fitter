#include <cstdlib>
#include <iostream>
#include "fitting_settings.hpp"
#include "ltc_fitting.hpp"
#include "result_saving.hpp"
#include "ltc_nelder_mead.hpp"
#include "brdf_plot.hpp"
#include "ggx.hpp"
#include "ltc.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

int main(int argc, const char* argv[])
{
    ggx ggx;

    ltc ltc;
    ltc.set_ltc_matrix({
        {0.1f, 0.0f, 0.0f},
        {1.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.5f}
    });

    brdf_plot plot;
    plot.export_png(&ggx, "test.png");
    plot.export_png(&ltc, "test_ltc.png");

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
