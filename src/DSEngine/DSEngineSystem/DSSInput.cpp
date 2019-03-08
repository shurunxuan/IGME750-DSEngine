#include "DSSInput.h"
#include "DSFLogging.h"

DSSInput* SInput = nullptr;

DSSInput::DSSInput()
{
	SInput = this;
}


DSSInput::~DSSInput()
{
}

void DSSInput::Init(HWND hWnd)
{
	xInput.Init();
	rawInput.Init(hWnd);

	LOG_TRACE << "DS Engine Input System Initialized!";
}

void DSSInput::Update()
{
	xInput.Update();
	rawInput.Update();

	// TODO: Delete this part. This is only for test.
	for (int player = 0; player < XUSER_MAX_COUNT; ++player)
		for (unsigned int i = 0; i < sizeof(WORD) * 8; ++i)
		{
			const DSButtonCode buttonCode = DSFXInput::GetButtonCode(1 << i);

			if (xInput.GetButtonDown(buttonCode, player))
			{
				LOG_TRACE << DSFXInput::GetButtonName(buttonCode) << " on player " << player << " pressed.";
			}
			if (xInput.GetButtonUp(buttonCode, player))
			{
				LOG_TRACE << DSFXInput::GetButtonName(buttonCode) << " on player " << player << " released.";
			}
		}

	for (int key = 0; key < 256; ++key)
	{
		if (rawInput.GetKeyDown(key))
		{
			LOG_TRACE << DSFRawInput::GetKeyName(key) << " on keyboard pressed.";
		}
		if (rawInput.GetKeyUp(key))
		{
			LOG_TRACE << DSFRawInput::GetKeyName(key) << " on keyboard released.";
		}
	}

	for (int button = 0; button < 5; ++button)
	{
		if (rawInput.GetMouseButtonDown(button))
		{
			LOG_TRACE << "Button " << button << " on mouse pressed at position (" << rawInput.GetMousePosX() << ", " << rawInput.GetMousePosY() << ")";
		}
		if (rawInput.GetMouseButtonUp(button))
		{
			LOG_TRACE << "Button " << button << " on mouse released at position (" << rawInput.GetMousePosX() << ", " << rawInput.GetMousePosY() << ")";
		}
	}
}
