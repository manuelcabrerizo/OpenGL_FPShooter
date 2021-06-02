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

struct IVec3
{
    int x, y, z;
};

struct Matrix
{
    float m[4][4];
};

struct Matrix3
{
    float m[3][3];
};


struct Quaternion
{
    float x, y, z, w;
};


Quaternion QuaternionRotationAxis(Vec3 axis, float angle);
Matrix GetQuaternionRotationMatrix(Quaternion q);
Quaternion GetQuaternionFromMatrix(Matrix m);
float quaternion_length(Quaternion v);
Quaternion normaliza_quaternion(Quaternion q);
Quaternion QuaternionInterpolate(Quaternion a, Quaternion b, float blend);

float to_radiant(float v);
float absf(float f);
int Abs(int number);
float MaxFloat(float a, float b);
float MinFloat(float a, float b);

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
Vec3 operator*(const Matrix& m, const Vec3& v);
Vec3 operator/(const Vec3& v0, const Vec3& v1);
Vec3 operator*(const Vec3& v0, const Vec3& v1);

Matrix operator+(const Matrix& a, const Matrix& b);
Matrix operator-(const Matrix& a, const Matrix& b);
Matrix operator*(const Matrix& a, const Matrix& b);
Matrix operator*(const Matrix& m, const float& s);
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

Matrix3 to_3x3_matrix(Matrix m);
Matrix to_4x4_matrix(Matrix3 m);
float det_3x3(Matrix3 m);
float det_4x4(Matrix m);
Matrix3 get_cof_matrix(Matrix m, int x, int y);
Matrix get_matrix_adjunta(Matrix m);
Matrix get_inverse_matrix(Matrix m);

float vec3_dot(Vec3 a, Vec3 b);
Vec3 vec3_cross(Vec3 a, Vec3 b);
float vec3_length(Vec3 v);
Vec3 normaliza_vec3(Vec3 v);
Vec3 PerpZVec3(Vec3 v);

#endif
