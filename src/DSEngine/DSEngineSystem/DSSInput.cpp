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

void DSSInput::Init()
{
	xInput.Init();
	rawInput.Init();

	LOG_TRACE << "DS Engine Input System Initialized!";
}

void DSSInput::Update()
{
	xInput.Update();
	rawInput.Update();
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
}
