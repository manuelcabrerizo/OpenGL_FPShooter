#include "line.h"
#include <glad/glad.h>

Line GenLine(Vec3 a, Vec3 b, Vec3 color, Shader* shader)
{
    Line newLine;
    newLine.a = a;
    newLine.b = b;
    newLine.vertices[0]  = a.x; 
    newLine.vertices[1]  = a.y;
    newLine.vertices[2]  = a.z;
    newLine.vertices[3]  = color.x; 
    newLine.vertices[4]  = color.y;
    newLine.vertices[5]  = color.z;
    newLine.vertices[6]  = b.x; 
    newLine.vertices[7]  = b.y;
    newLine.vertices[8]  = b.z;
    newLine.vertices[9]  = color.x; 
    newLine.vertices[10] = color.y;
    newLine.vertices[11] = color.z;
    newLine.shader = *shader;

    glGenVertexArrays(1, &newLine.vao);
    glBindVertexArray(newLine.vao);
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(newLine.vertices), newLine.vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    return newLine;
}

void DrawLine(Line* line, Matrix model)
{
    UseShader(&line->shader);
    glBindVertexArray(line->vao);
    SetShaderMatrix(model, line->shader.worldMatLoc);
    glDrawArrays(GL_LINES, 0, 2); 
}

Plane GenPlane(Vec3 a, Vec3 b, Vec3 c, Vec3 color, Shader* shader)
{
    Plane newPlane;
    newPlane.a = a;
    newPlane.u = b - a;
    newPlane.v = c - a;

    for(int i = 0; i < 24; i += 6)
    {
        if(i < 6)
        {
            newPlane.vertices[i + 0] = 0.0f;
            newPlane.vertices[i + 1] = 0.0f;
            newPlane.vertices[i + 2] = 0.0f;
            newPlane.vertices[i + 3] = color.x;
            newPlane.vertices[i + 4] = color.y;
            newPlane.vertices[i + 5] = color.z;
        }
        else if(i >= 6 && i < 12)
        {
            newPlane.vertices[i + 0] = newPlane.u.x;
            newPlane.vertices[i + 1] = newPlane.u.y;
            newPlane.vertices[i + 2] = newPlane.u.z;
            newPlane.vertices[i + 3] = color.x;
            newPlane.vertices[i + 4] = color.y;
            newPlane.vertices[i + 5] = color.z;
        }
        else if(i >= 12 && i < 18)
        {
            newPlane.vertices[i + 0] = newPlane.v.x;
            newPlane.vertices[i + 1] = newPlane.v.y;
            newPlane.vertices[i + 2] = newPlane.v.z;
            newPlane.vertices[i + 3] = color.x;
            newPlane.vertices[i + 4] = color.y;
            newPlane.vertices[i + 5] = color.z;
        }
        else if(i >= 18)
        {
            newPlane.vertices[i + 0] = newPlane.u.x + newPlane.v.x;
            newPlane.vertices[i + 1] = newPlane.u.y + newPlane.v.y;
            newPlane.vertices[i + 2] = newPlane.u.z + newPlane.v.z;
            newPlane.vertices[i + 3] = color.x;
            newPlane.vertices[i + 4] = color.y;
            newPlane.vertices[i + 5] = color.z;
        }
        
    }

    newPlane.indices[0] = 0;
    newPlane.indices[1] = 3;
    newPlane.indices[2] = 2;
    newPlane.indices[3] = 0;
    newPlane.indices[4] = 1;
    newPlane.indices[5] = 3;

    newPlane.shader = *shader;
    glGenVertexArrays(1, &newPlane.vao);
    glBindVertexArray(newPlane.vao);
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(newPlane.vertices), newPlane.vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    unsigned ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), newPlane.indices, GL_STATIC_DRAW);

    return newPlane;
}

void DrawPlane(Plane* plane, Matrix model)
{
    UseShader(&plane->shader);
    glBindVertexArray(plane->vao);
    SetShaderMatrix(model, plane->shader.worldMatLoc);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


Vec3 Lerp(Vec3 a, Vec3 b, float t)
{       
    Vec3 v = b - a;
    Vec3 result = a + (v * t);
    return result;
}

Vec3 LerpPlane(Plane* plane, float s, float t)
{
    Vec3 result;
    result.x = plane->a.x + plane->v.x * s + plane->u.x * t;
    result.y = plane->a.y + plane->v.y * s + plane->u.y * t;
    result.z = plane->a.z + plane->v.z * s + plane->u.z * t;
    return result;
}

float LineIntersectsAt(Line* l1, Line* l2)
{
    Vec3 c = l2->a - l1->a;
    Vec3 u = l1->b - l1->a;
    Vec3 v = l2->b - l2->a; 
    float t = vec3_dot(PerpZVec3(v), c) / vec3_dot(PerpZVec3(v), u);
    return t;
}

float LinePlaneIntersectsAt(Line* line, Plane* plane)
{
    Vec3 n = vec3_cross(plane->u, plane->v);
    float t = vec3_dot(n, (plane->a - line->a)) / vec3_dot(n, (line->b - line->a));
    return t; 
}

Vec3 Reflect(Line* line, Vec3 normal)
{
    Vec3 norm = normaliza_vec3(normal);
    Vec3 vnorm = normaliza_vec3(line->b - line->a);
    float d = vec3_dot(norm, vnorm);
    if(d == 0) return (line->b - line->a);
    float vn2  = d * 2;
    Vec3 r = vnorm - norm * vn2;
    return r;
}

float Vec3PlaneIntersectsAt(Vec3 a, Vec3 b, Plane* plane)
{
    Vec3 n = vec3_cross(plane->u, plane->v);
    float t = vec3_dot(n, (plane->a - a)) / vec3_dot(n, (b - a));
    return t; 
}

Vec3 Vec3Reflect(Vec3 a, Vec3 b, Vec3 normal)
{
    Vec3 norm = normaliza_vec3(normal);
    Vec3 vnorm = normaliza_vec3(b - a);
    float d = vec3_dot(norm, vnorm);
    if(d == 0) return (b - a);
    float vn2  = d * 2;
    Vec3 r = vnorm - norm * vn2;
    return r;
}
