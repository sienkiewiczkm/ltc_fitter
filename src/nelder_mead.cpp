#include "nelder_mead.hpp"
#include "log.hpp"
#include <iostream>

const float nelder_mead::_reflection_coeff = 1.0f;
const float nelder_mead::_expansion_coeff = 2.0f;
const float nelder_mead::_contraction_coeff = 0.5f;
const float nelder_mead::_shrink_coeff = 0.5f;

nelder_mead::nelder_mead():
    _simplex{}
{
}

glm::vec4 nelder_mead::optimize(glm::vec4 start_parameters)
{
    auto x_opt = glm::vec4{};

    for (auto& i : _simplex)
    {
        i = start_parameters;
    }

    float delta = 0.2f;
    _simplex[1] += delta * glm::vec4{1.0f, 0.0f, 0.0f, 0.0f};
    _simplex[2] += delta * glm::vec4{0.0f, 1.0f, 0.0f, 0.0f};
    _simplex[3] += delta * glm::vec4{0.0f, 0.0f, 1.0f, 0.0f};
    _simplex[4] += delta * glm::vec4{0.0f, 0.0f, 0.0f, 1.0f};

    _iteration_epsilon = 0.0001f;

    auto iteration = 0;
    auto max_iterations = 300;

    for (iteration = 0; iteration < max_iterations; ++iteration)
    {
        if (!run_iteration())
        {
            x_opt = _simplex[_min_index];
            break;
        }
    }

    if (iteration >= max_iterations)
    {
        // iteration limit reached
        x_opt = _simplex[_min_index];
        log_warning() << "[Nelder-Mead] Iteration limit reached ("
            << max_iterations << ")." << std::endl;
    }
    else
    {
        log_info() << "[Nelder-Mead] Solution found after " << iteration + 1
            << " iterations." << std::endl;
    }

    return x_opt;
}

bool nelder_mead::run_iteration()
{
    find_min_max_error();

    if (get_simplex_norm() < _iteration_epsilon)
    {
        return false;
    }

    // stop if difference between maximum and minimum value is very low
    // read the source: Numerical Recipes in C++ (3rd Ed.)
    float a = fabsf(_min_error);
    float b = fabsf(_max_error);
    if (2.0f*fabsf(a - b) < (a + b)*(1e-5f))
        return false;

    auto center_of_mass = get_center_of_mass();

    auto pr = center_of_mass
        + _reflection_coeff * (center_of_mass - _simplex[_max_index]);
    auto pr_value = estimate_error(pr);

    // this almost maximum error comparison comes from Wikipedia article
    // refer: 'One possible variation of the NM algorithm' section
    if (pr_value >= _min_error && pr_value < _almost_max_error)
    {
        _simplex[_almost_max_index] = pr;
        return true;
    }
    else if (pr_value < _min_error)
    {
        auto pe = center_of_mass + _expansion_coeff * (pr - center_of_mass);
        auto pe_value = estimate_error(pe);

        if (pe_value < pr_value) // expansion
        {
            _simplex[_max_index] = pe;
        }
        else // reflection
        {
            _simplex[_max_index] = pr;
        }
    }
    else
    {
        if (_min_error <= pr_value && pr_value <= _max_error) // reflection
        {
            _simplex[_max_index] = pr;
        }
        else
        {
            auto pc = center_of_mass
                + _contraction_coeff * (_simplex[_max_index] - center_of_mass);
            auto pc_value = estimate_error(pc);

            if (pc_value >= _max_error)
            {
                reduce_simplex(_min_index);
            }
            else // contraction
            {
                _simplex[_max_index] = pc;
            }
        }
    }

    return true;
}

void nelder_mead::find_min_max_error()
{
    _min_error = std::numeric_limits<float>::max();
    _max_error = std::numeric_limits<float>::lowest();
    _almost_max_error = std::numeric_limits<float>::lowest();
    _min_index = _max_index = _almost_max_index = -1;

    for (auto i = 0; i < _simplex.size(); ++i)
    {
        float value = estimate_error(_simplex[i]);

        if (_min_error > value)
        {
            _min_error = value;
            _min_index = i;
        }

        if (_max_error < value)
        {
            _almost_max_error = _max_error;
            _almost_max_index = _max_index;
            _max_error = value;
            _max_index = i;
        }
        else if (_almost_max_error < value)
        {
            _almost_max_error = value;
            _almost_max_index = i;
        }
    }
}

float nelder_mead::get_simplex_norm()
{
    auto max_norm = 0.0f;

    for (auto i = 0; i < _simplex.size(); ++i)
    {
        auto current_norm = glm::length(_simplex[_min_index] - _simplex[i]);
        max_norm = std::max(max_norm, current_norm);
    }

    return max_norm;
}

glm::vec4 nelder_mead::get_center_of_mass()
{
    glm::vec4 center_of_mass{};
    for (auto i = 0; i < _simplex.size(); ++i)
        if (i != _max_index)
            center_of_mass += _simplex[i];

    center_of_mass = center_of_mass / static_cast<float>(_simplex.size()-1);

    return center_of_mass;
}

void nelder_mead::reduce_simplex(int around)
{
    for (auto i = 0; i < _simplex.size(); ++i)
    {
        if (i != around)
        {
            _simplex[i] = _simplex[around]
                + _shrink_coeff * (_simplex[i] - _simplex[around]);
        }
    }
}
