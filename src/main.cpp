#include <cstdlib>
#include <iostream>
#include <sstream>

#include "utils/log.hpp"
#include "fitting_settings.hpp"
#include "fitting/ltc_lookup_builder.hpp"
#include "files/arealights_ltc_file/arealights_ltc_file.hpp"
#include "exporters/alf_exporter.hpp"
#include "tests/tests.hpp"
#include "tests/export_original_fit.hpp"
#include "preview.hpp"

void run_tests();

int main(int argc, const char *argv[])
{
  try
  {
    fitting_settings settings;
    if (!get_fitting_settings_from_command_line(settings, argc, argv))
    {
      log_error() << "Failed to set fitting settings, aborting..." << std::endl;
      return EXIT_FAILURE;
    }

    if (settings.run_mode == run_mode::exit) {
      return EXIT_SUCCESS;
    }
    else if (settings.run_mode == run_mode::test) {
      run_tests();
      return EXIT_SUCCESS;
    } else if (settings.run_mode == run_mode::preview_input) {
      create_preview_for_file(settings.input_file);
      return EXIT_SUCCESS;
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

void run_tests()
{
  test_all();
  export_alf_original_fit("original_heitz.ltc");
}
