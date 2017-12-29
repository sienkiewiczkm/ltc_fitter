#include <cstdlib>
#include <iostream>
#include "fitting_settings.hpp"
#include "fitting/ltc_lookup_builder.hpp"

#define GLM_ENABLE_EXPERIMENTAL

#include "glm/gtx/string_cast.hpp"

#include <boost/math/constants/constants.hpp>
#include "utils/log.hpp"

#include "files/arealights_ltc_file/arealights_ltc_file.hpp"
#include "exporters/alf_exporter.hpp"

int main(int argc, const char *argv[])
{
  try
  {
    fitting_settings settings;
    if (!get_fitting_settings_from_command_line(settings, argc, argv))
    {
      log_error() << "failed to set fitting settings, aborting" << std::endl;
      return EXIT_FAILURE;
    }

    print_fitting_settings(settings);
    auto result = build_lookup(settings);

    auto file = make_alf_file(result);
    alf::write_alf_file(*file, settings.output_file);
  }
  catch (std::exception &exc)
  {
    log_error() << "Exception: " << exc.what() << std::endl;
    return EXIT_FAILURE;
  }
  catch (...)
  {
    log_error() << "Unknown exception has been thrown. Aborting." << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
