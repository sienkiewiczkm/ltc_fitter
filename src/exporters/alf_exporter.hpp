#pragma once

#include "../files/arealights_ltc_file/arealights_ltc_file.hpp"
#include "../fitting/ltc_lookup_builder.hpp"
#include "../fitting/fitting_result.hpp"

std::unique_ptr<alf::alf_file> make_alf_file(const fitting_result& data);