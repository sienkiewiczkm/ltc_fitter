#pragma once
#include <memory>
#include <string>
#include <vector>

namespace alf
{

/*
 * ALF file structure:
 *
 * Header
 *   Total size: 16 bytes
 *          CHAR[4]   identifier            = "ALFS"
 *          SHORT     version major         =1
 *          SHORT     version_minor         =0
 *          UINT      num_samples_angle
 *          UINT      num_samples_roughness
 *
 * Texture Slot 1
 *   Total size: (num_samples_angle * num_samples_roughness * 4 * sizeof(float)) bytes
 *   base_index(angle_id, roughness_id) = 4 * (num_samples_roughness * angle_id + roughness_id)
 *          FLOAT     matrix coefficient [0][0]
 *          FLOAT     matrix coefficient [0][2]
 *          FLOAT     matrix coefficient [1][1]
 *          FLOAT     matrix coefficient [2][0]
 *
 * Texture Slot 2
 *   Total size: (num_samples_angle * num_samples_roughness * 3 * sizeof(float)) bytes
 *   base_index(angle_id, roughness_id) = 3 * (num_samples_roughness * angle_id + roughness_id)
 *          FLOAT     matrix coefficient [2][2]
 *          FLOAT     ltc magnitude
 *          FLOAT     ltc fresnel factor
 *
 */

struct alf_header
{
  char id[4];
  short version_major;
  short version_minor;
  unsigned int num_samples_angle;
  unsigned int num_samples_roughness;
};

struct alf_file
{
  alf_header header;
  std::vector<float> texture_slot1;
  std::vector<float> texture_slot2;
};

std::unique_ptr<alf_file> read_alf_file(const std::string &filename);
int write_alf_file(const alf_file &file, const std::string &filename);

}