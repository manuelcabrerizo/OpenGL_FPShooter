#include "math.h"
#include <math.h>

Quaternion QuaternionRotationAxis(Vec3 axis, float angle)
{
    axis = normaliza_vec3(axis);
    float w = cosf(angle / 2.0f);
    float s = sinf(angle / 2.0f);
    Vec3 qv = {axis.x * s, axis.y * s, axis.z * s};
    Quaternion q = {qv.x, qv.y, qv.z, w};
    return q;
}

Matrix GetQuaternionRotationMatrix(Quaternion q)
{
    Matrix result = {{
        {1.0f-(2.0f*q.y*q.y)-(2.0f*q.z*q.z), (2.0f*q.x*q.y)-(2.0f*q.w*q.z), (2.0f*q.x*q.z)+(2.0f*q.w*q.y), 0.0f},
        {(2.0f*q.x*q.y)+(2.0f*q.w*q.z), 1.0f-(2.0f*q.x*q.x)-(2.0f*q.z*q.z), (2.0f*q.y*q.z)-(2.0f*q.w*q.x), 0.0f},
        {(2.0f*q.x*q.z)-(2.0f*q.w*q.y), (2.0f*q.y*q.z)+(2.0f*q.w*q.x), 1.0f-(2.0f*q.x*q.x)-(2.0f*q.y*q.y), 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    }};
    return result;
}

Quaternion GetQuaternionFromMatrix(Matrix m)
{
    float angle = acosf(0.5f*(m.m[0][0]+m.m[1][1]+m.m[2][2]+m.m[3][3]-2.0f));
    float axisX = (m.m[2][1]-m.m[1][2])/(2.0f*sinf(angle));
    float axisY = (m.m[0][2]-m.m[2][0])/(2.0f*sinf(angle));
    float axisZ = (m.m[1][0]-m.m[0][1])/(2.0f*sinf(angle));
    Vec3 axis = {axisX, axisY, axisZ};
    Quaternion result = QuaternionRotationAxis(axis, angle);
    return result;
}

float to_radiant(float v)
{
    return v * PI / 180.0f;
}

float absf(float f)
{
    if(f < 0)
    {
        f = -f;
    }
    return f;
}

int Abs(int number)
{
    if((number & 1 << 31) != 0) 
        return (number & ~(1 << 31)) - (2147483648 + (number << 1)); 
    return number;
}


float MaxFloat(float a, float b)
{
    if(a > b)
        return a;
    return b;
}

float MinFloat(float a, float b)
{
    if(a < b)
        return a;
    return b;
}

Vec3 vec3_rotate_y(Vec3 v, float angle)
{
    Vec3 rotatedVector;
        rotatedVector.x = v.x * cosf(angle) + v.z * sinf(angle);
        rotatedVector.y = v.y;
        rotatedVector.z = -v.x * sinf(angle) + v.z * cosf(angle); 
    return rotatedVector;
}


bool operator==(const Vec3& a, const Vec3& b)
{
    if(a.x == b.x && a.y == b.y && a.z == b.z)
    {
        return true;
    }
    return false;
}


bool operator!=(const Vec3& a, const Vec3& b)
{
    if(a.x != b.x || a.y != b.y || a.z == b.z)
    {
        return true;
    }
    return false;

}

Vec3 operator+(const Vec3& a, const Vec3& b)
{
    Vec3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

Vec3 operator-(const Vec3& a, const Vec3& b)
{
    Vec3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

void operator+=(Vec3& a, const Vec3& b)
{
    a.x = a.x + b.x;
    a.y = a.y + b.y;
    a.z = a.z + b.z; 
}

void operator-=(Vec3& a, const Vec3& b)
{
    a.x = a.x - b.x;
    a.y = a.y - b.y;
    a.z = a.z - b.z; 
}

Vec3 operator+(const Vec3& v, const float& s)
{
    Vec3 result;
    result.x = v.x + s;
    result.y = v.y + s;
    result.z = v.z + s;
    return result;
 
}

Vec3 operator-(const Vec3& v, const float& s)
{
    Vec3 result;
    result.x = v.x - s;
    result.y = v.y - s;
    result.z = v.z - s;
    return result;
}

Vec3 operator*(const Vec3& v, const float& s)
{
    Vec3 result;
    result.x = v.x * s;
    result.y = v.y * s;
    result.z = v.z * s;
    return result;
}

Vec3 operator/(const Vec3& v, const float& s)
{
    Vec3 result;
    result.x = v.x / s;
    result.y = v.y / s;
    result.z = v.z / s;
    return result;
}

Vec3 operator-(const Vec3& v)
{
    Vec3 result;
    v.x != 0 ? result.x = -v.x : result.x = v.x;
    v.y != 0 ? result.y = -v.y : result.y = v.y;
    v.z != 0 ? result.z = -v.z : result.z = v.z;
    return result;
}

Vec3 operator/(const Vec3& v0, const Vec3& v1)
{
    Vec3 result;
    result.x = v0.x / v1.x;
    result.y = v0.y / v1.y;
    result.z = v0.z / v1.z;
    return result;
}


Vec3 operator*(const Vec3& v0, const Vec3& v1)
{
    Vec3 result;
    result.x = v0.x * v1.x;
    result.y = v0.y * v1.y;
    result.z = v0.z * v1.z;
    return result;
}


Matrix operator+(const Matrix& a, const Matrix& b)
{
    Matrix result;
    for(int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            result.m[y][x] = a.m[y][x] + b.m[y][x];
        }
    }
    return result;
}

Matrix operator-(const Matrix& a, const Matrix& b)
{
    Matrix result;
    for(int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            result.m[y][x] = a.m[y][x] - b.m[y][x];
        }
    }
    return result;
}

Matrix operator*(const Matrix& a, const Matrix& b)
{
    Matrix result;
    for(int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
                result.m[y][x] =
                a.m[y][0] * b.m[0][x] +
                a.m[y][1] * b.m[1][x] +
                a.m[y][2] * b.m[2][x] +
                a.m[y][3] * b.m[3][x];
        }
    }
    return result;
}


Matrix operator*(const Matrix& m, const float& s)
{
    Matrix result;
    for(int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            result.m[y][x] = m.m[y][x] * s; 
        }
    }
    return result;
}

Matrix get_view_matrix(Vec3 eye, Vec3 target, Vec3 up)
{
    Vec3 z = normaliza_vec3(target - eye);
    Vec3 x = normaliza_vec3(vec3_cross(up, z));
    Vec3 y = vec3_cross(z, x);
    Matrix result = {{
        { x.x,              y.x,              z.x,             0.0f},
        { x.y,              y.y,              z.y,             0.0f},
        { x.z,              y.z,              z.z,             0.0f},
        {-vec3_dot(x, eye), -vec3_dot(y, eye), -vec3_dot(z, eye), 1.0f}
    }}; 
    return result; 
}

Matrix get_projection_perspective_matrix(float fov, float aspect, float znear, float zfar)
{
    float yScale = 1/tanf(fov/2);
    float xScale = yScale / aspect;
    float a = zfar/(zfar-znear);
    float b = -znear*zfar/(zfar-znear);
    Matrix result = {{
        { xScale,   0.0f, 0.0f, 0.0f},
        {   0.0f, yScale, 0.0f, 0.0f},
        {   0.0f,   0.0f,    a, 1.0f},
        {   0.0f,   0.0f,    b, 0.0f}
    }}; 
    return result;
}

Matrix get_projection_orthogonal_matrix(int width, int height, float znear, float zfar)
{
    float w = (float)width;
    float h = (float)height;
    float l = 0;
    float r = w;
    float b = 0;
    float t = h;

    Matrix result = {{
        {2.0f / (r - l),    0.0f,              0.0f,                   0.0f},
        {0.0f,              2.0f / (t - b),    0.0f,                   0.0f},
        {0.0f,              0.0f,              1.0f / (zfar - znear),  0.0f},
        {(l + r) / (l - r), (t + b) / (b - t), znear / (znear - zfar), 1.0f}
    }};

    return result;
}

Matrix get_identity_matrix()
{
    Matrix result = {{
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    }};
    return result;
}

Matrix3 to_3x3_matrix(Matrix m)
{
     Matrix3 result = {{
        {m.m[0][0], m.m[0][1], m.m[0][2]},
        {m.m[1][0], m.m[1][1], m.m[1][2]},
        {m.m[2][0], m.m[2][1], m.m[2][2]}
     }};
    return result; 
}

Matrix to_4x4_matrix(Matrix3 m)
{
     Matrix result = {{
        {m.m[0][0], m.m[0][1], m.m[0][2], 0.0f},
        {m.m[1][0], m.m[1][1], m.m[1][2], 0.0f},
        {m.m[2][0], m.m[2][1], m.m[2][2], 0.0f},
        {0.0f,      0.0f,      0.0f,      1.0f}
     }};
    return result; 
}

Matrix matrix_transpose(Matrix m)
{
    Matrix result = {{
        {m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0]},
        {m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1]},
        {m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2]},
        {m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]}
    }};
    return result;
}

Matrix get_scale_matrix(Vec3 v)
{
    Matrix result = {{
        {v.x,  0.0f, 0.0f, 0.0f},
        {0.0f, v.y,  0.0f, 0.0f},
        {0.0f, 0.0f, v.z,  0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    }};
    result = matrix_transpose(result);
    return result;
}

Matrix get_translation_matrix(Vec3 v)
{
    Matrix result = {{
        {1.0f, 0.0f, 0.0f, v.x},
        {0.0f, 1.0f, 0.0f, v.y},
        {0.0f, 0.0f, 1.0f, v.z},
        {0.0f, 0.0f, 0.0f, 1.0f}
    }};
    result = matrix_transpose(result);
    return result;
}

Matrix get_rotation_x_matrix(float angle)
{
    Matrix result = {{
        {1.0f, 0.0f,         0.0f,        0.0f},
        {0.0f, cosf(angle), -sinf(angle), 0.0f},
        {0.0f, sinf(angle),  cosf(angle), 0.0f},
        {0.0f, 0.0f,         0.0f,        1.0f}
    }};
    result = matrix_transpose(result);
    return result;
}

Matrix get_rotation_y_matrix(float angle)
{
    Matrix result = {{
        { cosf(angle), 0.0f, sinf(angle), 0.0f},
        { 0.0f,        1.0f, 0.0f,        0.0f},
        {-sinf(angle), 0.0f, cosf(angle), 0.0f},
        { 0.0f,        0.0f, 0.0f,        1.0f}
    }};
    result = matrix_transpose(result);
    return result;
}

Matrix get_rotation_z_matrix(float angle)
{
    Matrix result = {{
        {cosf(angle), -sinf(angle), 0.0f, 0.0f},
        {sinf(angle),  cosf(angle), 0.0f, 0.0f},
        {0.0f,         0.0f,        1.0f, 0.0f},
        {0.0f,         0.0f,        0.0f, 1.0f}
    }};
    result = matrix_transpose(result);
    return result;
}


Matrix get_rotation_arbitrary_matrix(Vec3 n, float a)
{
    Matrix result = {{
        {powf(n.x, 2)*(1.0f-cosf(a))+cosf(a),  n.x*n.y*(1.0f-cosf(a))+n.z*sinf(a),   n.x*n.z*(1.0f-cosf(a))-n.y*sinf(a),     0.0f},
        {n.x*n.y*(1.0f-cosf(a))-n.z*sinf(a),   powf(n.y, 2)*(1.0f-cosf(a))+cosf(a),  n.y*n.z*(1.0f-cosf(a))+n.x*sinf(a),     0.0f},
        {n.x*n.z*(1.0f-cosf(a))+n.y*sinf(a),   n.y*n.z*(1.0f-cosf(a))-n.x*sinf(a),   powf(n.z, 2)*(1.0f-cosf(a))+cosf(a),    0.0f},
        {0.0f,                                 0.0f,                                 0.0f,                                   1.0f}
    }};
    return result;
}

float det_3x3(Matrix3 m)
{
    float det = (m.m[0][0]*m.m[1][1]*m.m[2][2]) +
                (m.m[0][1]*m.m[1][2]*m.m[2][0]) +
                (m.m[0][2]*m.m[1][0]*m.m[2][1]) -
                (m.m[0][2]*m.m[1][1]*m.m[2][0]) -
                (m.m[0][1]*m.m[1][0]*m.m[2][2]) -
                (m.m[0][0]*m.m[1][2]*m.m[2][1]);
    return det;
}

float det_4x4(Matrix m)
{
    float cof0 =  det_3x3(get_cof_matrix(m, 0, 0));
    float cof1 = -det_3x3(get_cof_matrix(m, 1, 0));
    float cof2 =  det_3x3(get_cof_matrix(m, 2, 0));
    float cof3 = -det_3x3(get_cof_matrix(m, 3, 0));
    float det4x4 = (m.m[0][0] * cof0) +
                   (m.m[0][1] * cof1) +
                   (m.m[0][2] * cof2) +
                   (m.m[0][3] * cof3);
    return det4x4;
}

Matrix3 get_cof_matrix(Matrix m, int x, int y)
{
    Matrix3 result;
    int yIndex = 0; 
    for(int i = 0; i < 4; i++)
    {
        if(i != y)
        {
            int xIndex = 0; 
            for(int j = 0; j < 4; j++)
            {
                if(j != x)
                {
                    result.m[yIndex][xIndex] = m.m[i][j];
                    xIndex++;
                }
                
            }
            yIndex++;
        }
    }    
    return result;
}

Matrix get_matrix_adjunta(Matrix m)
{
    Matrix result;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(i % 2 == 0)
            {
                if(j % 2 == 0)
                    result.m[i][j] =  det_3x3(get_cof_matrix(m, j, i));
                else
                    result.m[i][j] = -det_3x3(get_cof_matrix(m, j, i));
            }
            else
            {
                if(j % 2 == 0)
                    result.m[i][j] = -det_3x3(get_cof_matrix(m, j, i));
                else
                    result.m[i][j] =  det_3x3(get_cof_matrix(m, j, i));
            }
            
          }
    }
    result = matrix_transpose(result);
    return result;
}

Matrix get_inverse_matrix(Matrix m)
{
    Matrix result = get_matrix_adjunta(m) * (1.0f / det_4x4(m));
    return result;
}


float vec3_dot(Vec3 a, Vec3 b)
{
    float result = (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
    return result;
}

Vec3 vec3_cross(Vec3 a, Vec3 b)
{
    Vec3 result;
    result.x = (a.y * b.z) - (a.z * b.y);
    result.y = (a.z * b.x) - (a.x * b.z);
    result.z = (a.x * b.y) - (a.y * b.x);
    return result;
}

float vec3_length(Vec3 v)
{
    return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
}

Vec3 normaliza_vec3(Vec3 v)
{
    Vec3 result;
    float lenght = vec3_length(v);
    result.x = v.x / lenght;
    result.y = v.y / lenght;
    result.z = v.z / lenght;
    return result;
}

Vec3 PerpZVec3(Vec3 v)
{
    Vec3 result;
    result.x = -v.z;
    result.z =  v.x;
    return result;
}


Vec3 operator*(const Matrix& m, const Vec3& v)
{
    Vec3 result;
    result.x = (m.m[0][0] * v.x) + (m.m[0][1] * v.y) + (m.m[0][2] * v.z) + (m.m[0][3]);
    result.y = (m.m[1][0] * v.x) + (m.m[1][1] * v.y) + (m.m[1][2] * v.z) + (m.m[1][3]);
    result.z = (m.m[2][0] * v.x) + (m.m[2][1] * v.y) + (m.m[2][2] * v.z) + (m.m[2][3]); 
    result.z = (m.m[3][0] * v.x) + (m.m[3][1] * v.y) + (m.m[3][2] * v.z) + (m.m[3][3]);
    return result;
}



