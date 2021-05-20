#include "input.h"
#include "windows.h"
#include "xinput.h"

void SetMouseButtonPress(Input* input, int button)
{
    input->mouseButtons[button] = true;
}

void SetMouseButtonUp(Input* input, int button)
{
    input->mouseButtons[button] = false;
}

bool GetMouseButtonPress(Input* input, int button)
{
    return input->mouseButtons[button];     
}

void SetKeyDown(Input* input, uint32_t keyCode)
{
    input->keys_down[keyCode] = true;
    input->keys_up[keyCode] = false;
}

void SetKeyUp(Input* input, uint32_t keyCode)
{
    input->keys_down[keyCode] = false;
    input->keys_up[keyCode] = true;
}

bool GetKeyDown(Input* input, uint32_t keyCode)
{
    return input->keys_down[keyCode];
}

bool GetKeyUp(Input* input, uint32_t keyCode)
{
    return input->keys_up[keyCode];
}

void InitializeGamePads(Input* input)
{
    DWORD dwResult;
    for(DWORD i = 0; i < XUSER_MAX_COUNT; i++)
    {
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));
        dwResult = XInputGetState(i, &state);
        if(dwResult == ERROR_SUCCESS)
        {
            input->gamePadConected = true;
            return;
        }
    }
    input->gamePadConected = false;
}

void ProcessGamePads(Input* input)
{
    DWORD dwResult;
    for(DWORD i = 0; i < XUSER_MAX_COUNT; i++)
    {
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));
        dwResult = XInputGetState(i, &state);
        if(dwResult == ERROR_SUCCESS)
        {
            XINPUT_GAMEPAD* pad = &state.Gamepad;
            input->up = (pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
            input->down = (pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
            input->left = (pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
            input->right = (pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
            input->A = (pad->wButtons & XINPUT_GAMEPAD_A);
            input->B = (pad->wButtons & XINPUT_GAMEPAD_B);
            input->X = (pad->wButtons & XINPUT_GAMEPAD_X);
            input->Y = (pad->wButtons & XINPUT_GAMEPAD_Y);
            input->thumbLX = pad->sThumbLX;
            input->thumbLY = pad->sThumbLY;
            input->thumbRX = pad->sThumbRX;
            input->thumbRY = pad->sThumbRY; 
        }
    }   
}


