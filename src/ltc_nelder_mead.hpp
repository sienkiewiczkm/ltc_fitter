#pragma once
#include "nelder_mead.hpp"
#include "brdf.hpp"

class ltc_nelder_mead: public nelder_mead
{
public:
    ltc_nelder_mead(const brdf& brdf);
    virtual ~ltc_nelder_mead() {}

    void set_amplitude(float amplitude) { _amplitude = amplitude; }
    void set_view_dir(const glm::vec3& view_dir) { _view_dir = view_dir; }

protected:
    virtual float estimate_error(glm::vec4 parameters);
    double estimate_partial_error(
        const brdf& sample_source,
        const brdf& other_brdf,
        const glm::vec2& random_parameters
    );

private:
    const brdf& _brdf;
    float _amplitude;
    glm::vec3 _view_dir;
};
