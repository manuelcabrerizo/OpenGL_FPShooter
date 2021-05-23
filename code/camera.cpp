#include "camera.h"
#include <math.h>

void InitializeCamera(Camera* camera)
{
    camera->position = {0.0f, 1.4f, -1.0f}; 
    camera->target   = {0.0f, 1.4f, 1.0f};
    camera->up       = {0.0f, 1.0f, 0.0f};
    camera->right    = vec3_cross(camera->up, camera->target);
    camera->front    = vec3_cross(camera->up, camera->right);
    camera->viewMat  = get_view_matrix(camera->position, camera->position + camera->target, camera->up);
    camera->pitch    = 0.0f;
    camera->yaw      = 90.0f;
}

void UpdateCamera(Camera* camera, Input* input, float deltaTime)
{ 
    // Get the input of x and y offset of the MOUSEMOVE event
    camera->pitch -= (input->mouseIncY * 0.1f);// * deltaTime;  // Y movement of the mouse
    camera->yaw   -= (input->mouseIncX * 0.1f);// * deltaTime;  // X movement of the mouse
    // adjust the y max value 
    if(camera->pitch  >  89.0f)
        camera->pitch =  89.0f;
    if(camera->pitch  < -89.0f)
        camera->pitch = -89.0f;
    // calculate all the vectors we need to move the camera in diferents directions
    camera->target.x = cosf(to_radiant(camera->yaw)) * cosf(to_radiant(camera->pitch));
    camera->target.y = sinf(to_radiant(camera->pitch));
    camera->target.z = sinf(to_radiant(camera->yaw)) * cosf(to_radiant(camera->pitch));
    camera->front.x  = cosf(to_radiant(camera->yaw)); 
    camera->front.z  = sinf(to_radiant(camera->yaw));
    camera->right    = vec3_cross(camera->front, camera->up);
}

