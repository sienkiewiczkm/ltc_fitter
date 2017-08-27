#include "result_saving.hpp"
#include "ltc_fitting.hpp"
#include "stb_image_write.h"

static std::vector<unsigned char> discretize_color_vector(
    const std::vector<glm::vec4>& colors_floating
)
{
    std::vector<unsigned char> discretized;
    discretized.reserve(4 * colors_floating.size());

    for (auto i = 0; i < colors_floating.size(); ++i)
    {
        auto color = colors_floating[i];
        for (auto j = 0; j < 4; ++j)
        {
            auto clamped = std::max(0.0f, std::min(1.0f, color[j]));
            auto discr_channel = static_cast<unsigned char>(clamped * 255);
            discretized.push_back(discr_channel);
        }
    }
    return discretized;
}

void save_fitting_result(fitting_result result)
{
    auto discretized_texture = discretize_color_vector(result.transformations);

    stbi_write_png(
        result.settings.output_file.c_str(),
        result.settings.resolution,
        result.settings.resolution,
        4,
        discretized_texture.data(),
        4 * sizeof(unsigned char)
    );
}
