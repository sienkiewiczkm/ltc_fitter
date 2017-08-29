#include "tests.hpp"
#include <iostream>
#include "ggx.hpp"
#include "brdf_plot.hpp"
#include "ltc_fitting.hpp"
#include "log.hpp"
#include "ltc.hpp"

void test_export_simple_ltc()
{
    log_info() << "VISUAL TEST: test_export_simple_ltc started." << std::endl;

    ltc ltc;
    ltc.set_ltc_matrix(glm::mat3{1.0f});

    brdf_plot plot;
    plot.set_view_dir(glm::normalize(glm::vec3{1.0f, 0.0f, 1.0f}));
    plot.set_resolution(512);
    plot.export_png(&ltc, "test_export_simple_ltc.png");

    log_info() << "VISUAL TEST: expored to 'test_export_simple_ltc.png'"
        << std::endl;
    log_info() << "VISUAL TEST: test_export_simple_ltc completed." << std::endl;
}

void test_export_image()
{
    log_info() << "VISUAL TEST: test_export_image started." << std::endl;

    ggx ggx;
    ggx.set_alpha(0.2f);

    auto view_dir = glm::normalize(glm::vec3{1.0f, 0.0f, 1.0f});

    auto amplitude = compute_distribution_norm(ggx, view_dir, 32);
    log_info() << " amplitude: " << amplitude << std::endl;

    brdf_plot plot;
    plot.set_view_dir(view_dir);
    plot.set_resolution(512);
    plot.export_png(&ggx, "test_export_image.png");

    log_info() << "VISUAL TEST: test_export_image completed." << std::endl;
}

void test_all()
{
    test_export_simple_ltc();
    test_export_image();
}
