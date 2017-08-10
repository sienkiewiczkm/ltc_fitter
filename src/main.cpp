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
    float alpha = 0.4f;

    ggx ggx;
    ggx.set_alpha(alpha);

    glm::vec3 view_dir{-1.0f, 2.0f, 0.0f};
    view_dir = glm::normalize(view_dir);

    brdf_plot plot;
    plot.set_view_dir(view_dir);
    plot.export_png(&ggx, "test.png");

    std::cerr << "Fitting started." << std::endl;

    auto parameters = ltc_fit_single(ggx, view_dir);

    std::cerr << "Fitting complete, parameters: "
        << glm::to_string(parameters) << std::endl;

    ltc ltc;
    ltc.set_ltc_matrix({
        {parameters.x, parameters.y, 0.0f},
        {parameters.w, 1.0f, 0.0f},
        {0.0f, 0.0f, parameters.z}
    });

    plot.export_png(&ltc, "test_ltc.png");

    return EXIT_SUCCESS;
}
