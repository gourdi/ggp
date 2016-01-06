#ifndef __GGO_MATRIX4X4__
#define __GGO_MATRIX4X4__

#include <array>
#include <iostream>
#include <ggo_set3.h>

class ggo_matrix4x4
{
public:

    ggo_matrix4x4();
    ggo_matrix4x4(const ggo_matrix4x4 & matrix) = default;

    void    set_identity();
    void    set_translation(float dx, float dy, float dz);
    void    set_rotation_x(float angle);
    void    set_rotation_y(float angle);
    void    set_rotation_z(float angle);
    void    set_rotation(const ggo::vector3d_float & axis, float angle);
    void    set_projection(float fovy, float aspect, float near, float far);
    
    void    fill(float v);

    float   operator()(int x, int y) const;
    float & operator()(int x, int y);
    
    bool    compare(const ggo_matrix4x4 & matrix, float epsilon) const;

    const float * coefs() const { return &_coefs[0]; }
    
    ggo_matrix4x4 operator*(const ggo_matrix4x4 & matrix) const;

    ggo_matrix4x4 & operator=(const ggo_matrix4x4 & matrix) = default;

private:
    
    std::array<float, 16> _coefs;
};

// Math operators.
std::array<float, 4> operator*(const ggo_matrix4x4 & matrix, const std::array<float, 4> & v);

// IO operators.
std::ostream & operator<<(std::ostream& os, const ggo_matrix4x4 & matrix);
std::ostream & operator<<(std::ostream& os, const std::array<float, 4> & v);

#endif
