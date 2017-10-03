#pragma once
#include "glm/glm.hpp"
#include <vector>

class nelder_mead
{
public:
    nelder_mead(int space_dimmension);
    virtual ~nelder_mead() {}
    std::vector<float> optimize(std::vector<float> start_parameters);

protected:
    virtual float estimate_error(std::vector<float> parameters) = 0;

private:
    static const float _reflection_coeff;
    static const float _expansion_coeff;
    static const float _contraction_coeff;
    static const float _shrink_coeff;

    bool run_iteration();
    void find_min_max_error();
    float get_simplex_norm();
    std::vector<float> get_center_of_mass();
    void reduce_simplex(int around);

    std::vector<float> add(std::vector<float> a, std::vector<float> b) const;
    std::vector<float> sub(std::vector<float> a, std::vector<float> b) const;
    std::vector<float> mul(std::vector<float> input, float multipler) const;
    std::vector<float> div(std::vector<float> input, float divisor) const;
    float sum(std::vector<float> a) const;

    int _space_dimmension;
    float _iteration_epsilon;

    std::vector<std::vector<float>> _simplex;
    float _min_error, _max_error, _almost_max_error;
    int _min_index, _max_index, _almost_max_index;
};

