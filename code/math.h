#ifndef MATH_H
#define MATH_H

#define PI 3.14159265359

struct Vec2
{
    float x, y;
};

struct Vec3
{
    float x, y, z;
};

struct Matrix
{
    float m[4][4];
};

float to_radiant(float v);
float absf(float f);
int Abs(int number);

Vec3 operator+(const Vec3& a, const Vec3& b);
Vec3 operator-(const Vec3& a, const Vec3& b);
bool operator==(const Vec3& a, const Vec3& b);
bool operator!=(const Vec3& a, const Vec3& b);
void operator+=(Vec3& a, const Vec3& b);
void operator-=(Vec3& a, const Vec3& b);
Vec3 operator+(const Vec3& v, const float& s);
Vec3 operator-(const Vec3& v, const float& s);
Vec3 operator*(const Vec3& v, const float& s);
Vec3 operator/(const Vec3& v, const float& s);
Vec3 operator-(const Vec3& v);
Vec3 vec3_rotate_y(Vec3 v, float angle);

Matrix operator+(const Matrix& a, const Matrix& b);
Matrix operator-(const Matrix& a, const Matrix& b);
Matrix operator*(const Matrix& a, const Matrix& b);
Matrix get_view_matrix(Vec3 eye, Vec3 target, Vec3 up);
Matrix get_projection_perspective_matrix(float fov, float aspect, float znear, float zfar);
Matrix get_projection_orthogonal_matrix(int width, int height, float znear, float zfar);
Matrix get_identity_matrix();
Matrix matrix_transpose(Matrix m);
Matrix get_scale_matrix(Vec3 v);
Matrix get_translation_matrix(Vec3 v);
Matrix get_rotation_x_matrix(float angle);
Matrix get_rotation_y_matrix(float angle);
Matrix get_rotation_z_matrix(float angle);
Matrix get_rotation_arbitrary_matrix(Vec3 n, float angle);
float vec3_dot(Vec3 a, Vec3 b);
Vec3 vec3_cross(Vec3 a, Vec3 b);
float vec3_length(Vec3 v);
Vec3 normaliza_vec3(Vec3 v);
Vec3 PerpZVec3(Vec3 v);

#endif
