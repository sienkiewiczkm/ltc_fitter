#include "ltc.hpp"
#include "../utils/log.hpp"
#include "../utils/constants.hpp"

#define GLM_ENABLE_EXPERIMENTAL

#include "glm/gtx/string_cast.hpp"

ltc::ltc():
  _amplitude{1.0f},
  _ltc_matrix{},
  _ltc_matrix_inv{},
  _ltc_matrix_inv_determinant{1.0f}
{
}

float ltc::evaluate(
  const glm::vec3 &light_dir,
  const glm::vec3 &view_dir,
  float &probability_density_function
) const
{
  // here 'original' means original clamped cosine distribution 'space'
  glm::vec3 original_light_dir = glm::normalize(get_framed_ltc_matrix_inv() * light_dir);
  auto len = glm::length(get_framed_ltc_matrix() * original_light_dir);

  const float MIN_TRANSFORMED_LENGTH = 0.000001f;
  if (len < MIN_TRANSFORMED_LENGTH)
  {
    log_error() << "Transformed light direction to original space is degenerated (with length = 0)." << std::endl;
    throw std::logic_error("Transformed light direction is degenerated");
  }

  auto jacobian = glm::determinant(get_framed_ltc_matrix()) / (len * len * len);

  if (std::isnan(jacobian))
  {
    log_error() << "LTC Jacobian is NaN. Aborting." << std::endl;
    throw std::logic_error("Jacobian is NaN");
  }

  float d = std::max(0.0f, original_light_dir.z) / pi / jacobian;
  float result = _amplitude * d;

  probability_density_function = d;

  return result;
}

glm::vec3 ltc::sample(const glm::vec3 &view_dir, const glm::vec2 &random_parameters) const
{
  const float phi = std::acos(std::sqrt(random_parameters.x));
  const float theta = 2.0f * pi * random_parameters.y;

  const glm::vec3 original_sample{
    std::sin(phi) * std::cos(theta),
    std::sin(phi) * std::sin(theta),
    std::cos(phi)
  };

  return glm::normalize(get_framed_ltc_matrix() * original_sample);
}

void ltc::set_ltc_parameters(const glm::vec3 &parameters)
{
  set_ltc_matrix({
    {parameters.x, 0.0f,         0.0f},
    {0.0f,         parameters.y, 0.0f},
    {parameters.z, 0.0f,         1.0f}
  });
}

void ltc::set_base_frame(const glm::mat3 &base_frame)
{
  _base_frame = base_frame;
}

void ltc::set_ltc_matrix(const glm::mat3 &mat)
{
  _ltc_matrix = mat;
  _ltc_matrix_inv = glm::inverse(_ltc_matrix);
  _ltc_matrix_inv_determinant = glm::determinant(_ltc_matrix_inv);
}

const glm::mat3 &ltc::get_ltc_matrix() const
{
  return _ltc_matrix;
}

const glm::mat3 &ltc::get_ltc_matrix_inv() const
{
  return _ltc_matrix_inv;
}

glm::mat3 ltc::get_framed_ltc_matrix() const
{
  return (_base_frame * _ltc_matrix);
}

glm::mat3 ltc::get_framed_ltc_matrix_inv() const
{
  return glm::inverse(get_framed_ltc_matrix());
}

void ltc::set_amplitude(float amplitude)
{
  _amplitude = amplitude;
}

float ltc::get_amplitude() const
{
  return _amplitude;
}

ltc_store_data ltc::get_store_data() const
{
  auto framed_mtx = get_framed_ltc_matrix();

  float a = framed_mtx[0][0];
  float b = framed_mtx[0][2];
  float c = framed_mtx[1][1];
  float d = framed_mtx[2][0];
  float e = framed_mtx[2][2];

  // Rescaled inverse of m:
  // a 0 b   inverse  c*e     0     -b*c
  // 0 c 0     ==>     0  a*e - b*d   0
  // d 0 e           -c*d     0      a*c

  float t0 = c * e;
  float t1 = -b * c;
  float t2 = a * e - b * d;
  float t3 = -c * d;
  float t4 = a * c;

  ltc_store_data store_data;
  store_data.matrix_parameters = {t0, t1, t2, t3, t4};
  store_data.distribution_norm = _amplitude;
  store_data.fresnel_term = _fresnel;

  return store_data;
}

void ltc::set_store_data(const ltc_store_data &data)
{
  _amplitude = data.distribution_norm;
  _fresnel = data.fresnel_term;

  // TODO: Inverse may cause loss of data. It is done at least once during fitting process.
  const auto &mtx = data.matrix_parameters;
  set_ltc_matrix(glm::inverse(glm::mat3{
    {mtx[0], 0.0f,   mtx[1]},
    {0.0f,   mtx[2], 0.0f},
    {mtx[3], 0.0f,   mtx[4]}
  }));

  _base_frame = glm::mat4{};
}
