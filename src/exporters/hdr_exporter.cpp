#include "hdr_exporter.hpp"
#include "stb_image_write.h"

void export_to_hdr(std::string filename, const fitting_result &result)
{
  std::vector<float> data;

  for (const auto &transformation: result.transformations)
  {
    data.push_back(transformation.x);
    data.push_back(transformation.y);
    data.push_back(transformation.z);
    data.push_back(transformation.w);
  }

  stbi_write_hdr(
    filename.c_str(),
    result.settings.resolution,
    result.settings.resolution,
    4,
    data.data()
  );
}
