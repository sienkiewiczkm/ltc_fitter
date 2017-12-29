#include <iostream>
#include "export_original_fit.hpp"
#include "../fitting/ltc_lookup_builder.hpp"
#include "original_heitz_fit.hpp"
#include "../exporters/alf_exporter.hpp"

void export_alf_original_fit(const std::string &filename)
{
  fitting_result result;
  result.settings.resolution = original_heitz_fit::size;

  for (auto theta = 0; theta < original_heitz_fit::size; ++theta)
  {
    for (auto alpha = 0; alpha < original_heitz_fit::size; ++alpha)
    {
      auto index = original_heitz_fit::size * theta + alpha;
      const auto& mat = original_heitz_fit::matrix_inv_tab[index];

      ltc_store_data store_data = {};
      store_data.matrix_parameters = {mat[0][0], mat[0][2], mat[1][1], mat[2][0], mat[2][2]};
      store_data.distribution_norm = original_heitz_fit::magnitude_tab[index];
      store_data.fresnel_term = original_heitz_fit::fresnel_tab[index];

      result.transformations.push_back(store_data);
    }
  }

  auto alf_file = make_alf_file(result);
  alf::write_alf_file(*alf_file, filename);
}
