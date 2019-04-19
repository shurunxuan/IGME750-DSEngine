#include "DSFXInput.h"
#include "DSFLogging.h"


DSFXInput* FXInput = nullptr;

DSFXInput::DSFXInput()
{
	FXInput = this;

	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		ZeroMemory(&buttonState[i], sizeof(XINPUT_STATE));
		ZeroMemory(&buttonDownState[i], sizeof(XINPUT_STATE));
		ZeroMemory(&buttonUpState[i], sizeof(XINPUT_STATE));
		ZeroMemory(&lastState[i], sizeof(XINPUT_STATE));
		ZeroMemory(&syncButtonDownState[i], sizeof(XINPUT_STATE));
		ZeroMemory(&syncButtonUpState[i], sizeof(XINPUT_STATE));

		connected[i] = XInputGetState(i, &lastState[i]) == ERROR_DEVICE_NOT_CONNECTED;
	}
}


DSFXInput::~DSFXInput()
{
}

void DSFXInput::Init()
{
	OnDeviceChange();

	LOG_TRACE << "DS Engine Framework for XInput Initialized!";
}

void DSFXInput::Update()
{

	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		boost::unique_lock<boost::mutex> lock(mutex);

		ZeroMemory(&buttonState[i], sizeof(XINPUT_STATE));

		if (connected[i])
		{
			XInputGetState(i, &buttonState[i]);

			buttonDownState[i].Gamepad.wButtons |=
				buttonState[i].Gamepad.wButtons &
				~lastState[i].Gamepad.wButtons;

			buttonUpState[i].Gamepad.wButtons |=
				lastState[i].Gamepad.wButtons &
				~buttonState[i].Gamepad.wButtons;
		}

		lastState[i] = buttonState[i];
	}


}

void DSFXInput::SyncUpdate()
{
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		boost::unique_lock<boost::mutex> lock(mutex);

		ZeroMemory(&syncButtonDownState[i], sizeof(XINPUT_STATE));
		ZeroMemory(&syncButtonUpState[i], sizeof(XINPUT_STATE));

		if (connected[i])
		{
			syncButtonDownState[i] = buttonDownState[i];
			syncButtonUpState[i] = buttonUpState[i];
		}

		ZeroMemory(&buttonDownState[i], sizeof(XINPUT_STATE));
		ZeroMemory(&buttonUpState[i], sizeof(XINPUT_STATE));
	}
}

void DSFXInput::OnDeviceChange()
{
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		connected[i] = XInputGetState(i, &buttonState[i]) == ERROR_SUCCESS;
	}
}

bool DSFXInput::GetButton(DSJoystickButtonCode buttonCode, int player)
{
	bool result = false;
	if (player < 0)
	{
		for (int i = 0; i < XUSER_MAX_COUNT; ++i)
		{
			result |= GetButton(buttonCode, i);
		}
	}
	else
	{
		if (!connected[player]) return false;
		boost::unique_lock<boost::mutex> lock(mutex);
		result = (buttonState[player].Gamepad.wButtons & buttonCode) != 0;
	}
	return result;
}

bool DSFXInput::GetButtonDown(DSJoystickButtonCode buttonCode, int player)
{
	bool result = false;
	if (player < 0)
	{
		for (int i = 0; i < XUSER_MAX_COUNT; ++i)
		{
			result |= GetButtonDown(buttonCode, i);
		}
	}
	else
	{
		if (!connected[player]) return false;
		boost::unique_lock<boost::mutex> lock(mutex);
		result = (syncButtonDownState[player].Gamepad.wButtons & buttonCode) != 0;
	}
	return result;
}

bool DSFXInput::GetButtonUp(DSJoystickButtonCode buttonCode, int player)
{
	bool result = false;
	if (player < 0)
	{
		for (int i = 0; i < XUSER_MAX_COUNT; ++i)
		{
			result |= GetButtonUp(buttonCode, i);
		}
	}
	else
	{
		if (!connected[player]) return false;
		boost::unique_lock<boost::mutex> lock(mutex);
		result = (syncButtonUpState[player].Gamepad.wButtons & buttonCode) != 0;
	}
	return result;
}

WORD DSFXInput::GetDownEvent(int player)
{
	WORD result = 0;
	if (player < 0)
	{
		for (int i = 0; i < XUSER_MAX_COUNT; ++i)
		{
			result |= GetDownEvent(i);
		}
	}
	else
	{
		if (!connected[player]) return 0;
		boost::unique_lock<boost::mutex> lock(mutex);
		result = syncButtonDownState[player].Gamepad.wButtons;
	}
	return result;
}

WORD DSFXInput::GetUpEvent(int player)
{
	WORD result = 0;
	if (player < 0)
	{
		for (int i = 0; i < XUSER_MAX_COUNT; ++i)
		{
			result |= GetUpEvent(i);
		}
	}
	else
	{
		if (!connected[player]) return 0;
		boost::unique_lock<boost::mutex> lock(mutex);
		result = syncButtonUpState[player].Gamepad.wButtons;
	}
	return result;
}

float DSFXInput::GetAxis(DSJoystickAxisCode axisCode, int player, float deadZone) const
{
	if (player < 0) player = 0;
	if (!connected[player]) return 0.0f;
	if (deadZone < 0)
	{
		switch (axisCode)
		{
		case LT:
		case RT:
			deadZone = float(XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
				/ float(1 << sizeof(BYTE) * 8);
			break;
		case LX:
		case LY:
			deadZone = float(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
				/ float(1 << (sizeof(SHORT) * 8 - 1));
			break;
		case RX:
		case RY:
			deadZone = float(XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
				/ float(1 << (sizeof(SHORT) * 8 - 1));
			break;
		default:
			return 0.0f;
		}
	}

	const float rawData = GetRawAxis(axisCode, player);
	return abs(rawData) < deadZone ? 0.0f : rawData;
}

float DSFXInput::GetRawAxis(DSJoystickAxisCode axisCode, int player) const
{
	if (player < 0) player = 0;
	if (!connected[player]) return 0.0f;
	switch (axisCode)
	{
	case LT:
		return float(buttonState[player].Gamepad.bLeftTrigger) /
			float(1 << sizeof(BYTE) * 8);
	case RT:
		return float(buttonState[player].Gamepad.bRightTrigger) /
			float(1 << sizeof(BYTE) * 8);
	case LX:
		return float(buttonState[player].Gamepad.sThumbLX) /
			float(1 << (sizeof(SHORT) * 8 - 1));
	case LY:
		return float(buttonState[player].Gamepad.sThumbLY) /
			float(1 << (sizeof(SHORT) * 8 - 1));
	case RX:
		return float(buttonState[player].Gamepad.sThumbRX) /
			float(1 << (sizeof(SHORT) * 8 - 1));
	case RY:
		return float(buttonState[player].Gamepad.sThumbRY) /
			float(1 << (sizeof(SHORT) * 8 - 1));
	default:
		return 0.0f;
	}
	return -1;
}

void DSFXInput::SetControllerRumble(int controllerNum, float leftMotorSpeed, float rightMotorSpeed)
{
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = WORD(leftMotorSpeed * 65535); // use any value between 0-65535 here
	vibration.wRightMotorSpeed = WORD(rightMotorSpeed * 65535); // use any value between 0-65535 here
	if (controllerNum < 0)
	{
		for (int i = 0; i < XUSER_MAX_COUNT; ++i)
			XInputSetState(i, &vibration);
	}
	else
	{
		XInputSetState(controllerNum, &vibration);
	}
}

DSJoystickButtonCode DSFXInput::GetButtonCode(unsigned code)
{
	switch (code)
	{
	case 0x0001:
		return DPAD_UP;
	case 0x0002:
		return DPAD_DOWN;
	case 0x0004:
		return DPAD_LEFT;
	case 0x0008:
		return DPAD_RIGHT;
	case 0x0010:
		return START;
	case 0x0020:
		return BACK;
	case 0x0040:
		return LEFT_THUMB;
	case 0x0080:
		return RIGHT_THUMB;
	case 0x0100:
		return LEFT_SHOULDER;
	case 0x0200:
		return RIGHT_SHOULDER;
	case 0x1000:
		return A;
	case 0x2000:
		return B;
	case 0x4000:
		return X;
	case 0x8000:
		return Y;
	default:
		return RESERVED_0;
	}
}

DSJoystickAxisCode DSFXInput::GetAxisCode(unsigned code)
{
	switch (code)
	{
	case 0:
		return LT;
	case 1:
		return RT;
	case 2:
		return LX;
	case 3:
		return LY;
	case 4:
		return RX;
	case 5:
		return RY;
	default:
		return ERROR_AXIS;
	}
}

std::string DSFXInput::GetButtonName(DSJoystickButtonCode buttonCode)
{
	switch (buttonCode)
	{
	case DPAD_UP:
		return "DPad Up";
	case DPAD_DOWN:
		return "DPad Down";
	case DPAD_LEFT:
		return "DPad Left";
	case DPAD_RIGHT:
		return "DPad Right";
	case START:
		return "Start";
	case BACK:
		return "Back";
	case LEFT_THUMB:
		return "Left Stick";
	case RIGHT_THUMB:
		return "Right Stick";
	case LEFT_SHOULDER:
		return "Left Shoulder";
	case RIGHT_SHOULDER:
		return "Right Shoulder";
	case A:
		return "A Button";
	case B:
		return "B Button";
	case X:
		return "X Button";
	case Y:
		return "Y Button";
	default:
		return "ERROR Button";
	}
}

std::string DSFXInput::GetAxisName(DSJoystickAxisCode axisCode)
{
	switch (axisCode) {
	case LT:
		return "Left Trigger";
	case RT:
		return "Right Trigger";
	case LX:
		return "Left Stick X";
	case LY:
		return "Left Stick Y";
	case RX:
		return "Right Stick X";
	case RY:
		return "Right Stick Y";
	default:
		return "ERROR Axis";
	}
}
