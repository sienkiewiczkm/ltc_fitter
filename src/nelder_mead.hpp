#pragma once
#include "glm/glm.hpp"
#include <array>

class nelder_mead
{
public:
    nelder_mead();
    virtual ~nelder_mead() {}
    glm::vec4 optimize(glm::vec4 start_parameters);

protected:
    virtual float estimate_error(glm::vec4 parameters) = 0;

private:
    static const float _reflection_coeff;
    static const float _expansion_coeff;
    static const float _contraction_coeff;
    static const float _shrink_coeff;

    bool run_iteration();
    void find_min_max_error();
    float get_simplex_norm();
    glm::vec4 get_center_of_mass();
    void reduce_simplex(int around);

    float _iteration_epsilon;

    std::array<glm::vec4, 5> _simplex;
    float _min_error, _max_error;
    int _min_index, _max_index;
};

