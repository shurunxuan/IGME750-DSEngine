#include "DSFXInput.h"



DSFXInput::DSFXInput()
{
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		ZeroMemory(&state[i], sizeof(XINPUT_STATE));

		connected[i] = false;
	}
}


DSFXInput::~DSFXInput()
{
}

void DSFXInput::Update()
{
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		ZeroMemory(&state[i], sizeof(XINPUT_STATE));

		connected[i] = XInputGetState(i, &state[i]);
	}
}
