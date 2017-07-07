#pragma once
#include "nelder_mead.hpp"

class ltc_nelder_mead: public nelder_mead
{
public:
    ltc_nelder_mead();
    virtual ~ltc_nelder_mead() {}

protected:
    virtual float estimate_error(glm::vec4 parameters);
};
