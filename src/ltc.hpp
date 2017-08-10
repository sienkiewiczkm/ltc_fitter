#pragma once
#include "brdf.hpp"

class ltc: public brdf
{
public:
    ltc();
    virtual ~ltc() {}

    virtual float evaluate(
        const glm::vec3& light_dir,
        const glm::vec3& view_dir,
        float& probability_density_function
    ) const;

    virtual glm::vec3 sample(
        const glm::vec3& view_dir,
        const glm::vec2& random_parameters
    ) const;

    /*
     * Note about matrix and it's inverse. Here I assume that uninversed matrix
     * transformes vector from original (cosine) distribution to another one.
     */
    void set_ltc_matrix(const glm::mat3& mat);
    const glm::mat3& get_ltc_matrix() const;
    const glm::mat3& get_ltc_matrix_inv() const;

    void set_amplitude(float amplitude);
    float get_amplitude() const;

private:
    float _amplitude;
    float _ltc_matrix_inv_determinant;
    glm::mat3 _ltc_matrix;
    glm::mat3 _ltc_matrix_inv;
};
