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

    auto view_dir = glm::normalize(glm::vec3{0.0f, 0.0f, 1.0f});

    auto amplitude = compute_distribution_norm(ggx, view_dir, 32);
    log_info() << " amplitude: " << amplitude << std::endl;

    brdf_plot plot;
    plot.set_view_dir(view_dir);
    plot.set_resolution(512);
    plot.export_png(&ggx, "test_export_image.png");

    log_info() << "VISUAL TEST: test_export_image completed." << std::endl;
}

void test_untransformed_ltc_amplitude()
{
    ltc ltc;
    ltc.set_ltc_matrix(glm::mat3{1.0f});

    glm::vec3 view_dir{0.0f, 0.0f, 1.0f};
    auto amplitude = compute_distribution_norm(ltc, view_dir);

    log_info() << "Test - Untrasformed LTC norm started." << std::endl;
    log_info() << " Norm = " << amplitude << std::endl;
    log_info() << " This amplitude should be equal to 1.0" << std::endl;

    if ((amplitude - 1.0f) > 0.0001f)
    {
        log_error() << "Amplitude for untransformed LTC should be 1."
            << std::endl;
    }

    log_info() << "Test - Untrasformed LTC Amplitude completed." << std::endl;
}

void test_ggx_amplitude()
{
    ggx ggx;
    ggx.set_alpha(0.2f);

    glm::vec3 view_dir{0.0f, 0.0f, 1.0f};
    auto amplitude = compute_distribution_norm(ggx, view_dir);

    log_info() << "Test - GGX amplitude test started..." << std::endl;

    log_info() << " Norm = " << amplitude << std::endl;

    log_info() << "Test - GGX amplitude test completed." << std::endl;
}

void test_all()
{
    test_export_simple_ltc();
    test_export_image();
    test_untransformed_ltc_amplitude();
    test_ggx_amplitude();
}

