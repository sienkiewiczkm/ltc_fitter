#include "arealights_ltc_file.hpp"

std::unique_ptr<alf::alf_file> alf::read_alf_file(const std::string &filename)
{
  auto file = fopen(filename.c_str(), "rb");
  if (!file) {
    return nullptr;
  }

  alf::alf_header header;
  std::fread(&header, sizeof(alf::alf_header), 1, file);

  if (std::strcmp(header.id, "ALF") != 0) {
    return nullptr;
  }

  auto num_samples = header.num_samples_roughness * header.num_samples_angle;
  if (num_samples <= 0) {
    return nullptr;
  }

  std::vector<float> texture_slot1;
  texture_slot1.resize(4 * header.num_samples_angle * header.num_samples_roughness);
  std::fread(&texture_slot1[0], sizeof(float)*4, num_samples, file);

  std::vector<float> texture_slot2;
  texture_slot2.resize(3 * header.num_samples_angle * header.num_samples_roughness);
  std::fread(&texture_slot2[0], sizeof(float)*3, num_samples, file);

  std::fclose(file);

  auto alf_file = std::make_unique<alf::alf_file>();
  alf_file->header = std::move(header);
  alf_file->texture_slot1 = std::move(texture_slot1);
  alf_file->texture_slot2 = std::move(texture_slot2);
  return alf_file;
}

int alf::write_alf_file(const alf::alf_file &data, const std::string &filename)
{
  if (std::strcmp(data.header.id, "ALF") != 0) {
    return 2;
  }

  auto num_samples = data.header.num_samples_roughness * data.header.num_samples_angle;
  if (num_samples <= 0) {
    return 3;
  }

  if (data.texture_slot1.size() != (4*num_samples) || data.texture_slot2.size() != (3*num_samples)) {
    return 4;
  }

  auto file = fopen(filename.c_str(), "wb");
  if (!file) {
    return 1;
  }

  std::fwrite(&data.header, sizeof(alf::alf_header), 1, file);
  std::fwrite(&data.texture_slot1[0], sizeof(float)*4, num_samples, file);
  std::fwrite(&data.texture_slot2[0], sizeof(float)*3, num_samples, file);

  std::fclose(file);

  return 0;
}

