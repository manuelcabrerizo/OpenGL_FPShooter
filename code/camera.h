#ifndef CAMERA_H
#define CAMERA_H

#include <stdint.h>
#include "math.h"
#include "input.h"
#include "line.h"
#include "shader.h"

struct Camera
{
    Vec3 position;
    Vec3 right;
    Vec3 target;
    Vec3 up;
    Vec3 front;
    Matrix viewMat;
    float pitch;
    float yaw;
};

void InitializeCamera(Camera* camera);
void UpdateCamera(Camera* camera, Input* input, float deltaTime);

#endif
