#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

struct Input
{
    // keyboard...
    bool keys_down[256];
    bool keys_up[256];
    // mouse...
    uint32_t mouseDefaultPosX;
    uint32_t mouseDefaultPosY;
    float mousePosX;
    float mousePosY;
    float mouseIncX;
    float mouseIncY;
    bool mouseButtons[2];
    // gamePad...
    bool gamePadConected;
    bool up;
    bool down;
    bool left;
    bool right;
    bool A;
    bool B;
    bool X;
    bool Y; 
    int16_t thumbLX;
    int16_t thumbRX;
    int16_t thumbLY;
    int16_t thumbRY; 
};

// MOUSE HANDLE STUFF...
void SetMouseButtonPress(Input* input, int button);
void SetMouseButtonUp(Input* input, int button);
bool GetMouseButtonPress(Input* input, int button);

// KEYBOARD HANDLE STUFF...
void SetKeyDown(Input* input, uint32_t keyCode);
void SetKeyUp(Input* input, uint32_t keyCode);
bool GetKeyDown(Input* input, uint32_t keyCode);
bool GetKeyUp(Input* input, uint32_t keyCode);

// GAMEPAD HANDLE STUFF...
void InitializeGamePads(Input* input);
void ProcessGamePads(Input* input);

#endif

