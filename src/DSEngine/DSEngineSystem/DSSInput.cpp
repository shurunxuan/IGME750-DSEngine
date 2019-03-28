#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <utility>

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

void DSSInput::AsyncUpdate(float deltaTime)
{
	xInput.Update();
}

void DSSInput::SyncUpdate(float deltaTime)
{
	rawInput.Update();
	xInput.SyncUpdate();

	for (auto& input : inputs)
		input.Update(deltaTime);
}

bool DSSInput::GetButton(const std::string & name)
{
	bool result = false;
	for (const auto& input : inputs)
	{
		if (input.name == name)
			result |= input.getButton[0]() || input.getButton[1]();
	}
	return result;
}

bool DSSInput::GetButtonDown(const std::string & name)
{
	bool result = false;
	for (const auto& input : inputs)
	{
		if (input.name == name)
			result |= input.getButtonDown[0]() || input.getButtonDown[1]();
	}
	return result;
}

bool DSSInput::GetButtonUp(const std::string & name)
{
	bool result = false;
	for (const auto& input : inputs)
	{
		if (input.name == name)
			result |= input.getButtonUp[0]() || input.getButtonUp[1]();
	}
	return result;
}

float DSSInput::GetAxis(const std::string & name)
{
	float result = 0;
	for (const auto& input : inputs)
	{
		if (input.name == name)
			result += input.processedAxisValue;
	}
	return result;
}

float DSSInput::GetRawAxis(const std::string & name)
{
	float result = 0;
	for (const auto& input : inputs)
	{
		if (input.name == name)
			result += input.getRawAxis();
	}
	return result;
}

void DSSInput::GetMousePosition(float* x, float* y) const
{
	*x = float(rawInput.GetMousePosX());
	*y = float(rawInput.GetMousePosY());
}

void DSSInput::RegisterInput(std::string name, std::string posButton, std::string negButton, std::string altPosButton,
	std::string altNegButton, float gravity, float dead, float sensitivity, bool invert, InputType type,
	DSAxisCode axis, int joyNum)
{
	inputs.emplace_back(name, posButton, negButton, altPosButton, altNegButton, gravity, dead, sensitivity, invert, type, axis, joyNum, this);
}

DSJoystickButtonCode GetJoystickButtonCodeFromName(const std::string & buttonName)
{
	if (buttonName == "up")
		return DPAD_UP;
	if (buttonName == "down")
		return DPAD_DOWN;
	if (buttonName == "left")
		return DPAD_LEFT;
	if (buttonName == "right")
		return DPAD_RIGHT;
	if (buttonName == "start")
		return START;
	if (buttonName == "back")
		return BACK;
	if (buttonName == "ls")
		return LEFT_THUMB;
	if (buttonName == "rs")
		return RIGHT_THUMB;
	if (buttonName == "lb")
		return LEFT_SHOULDER;
	if (buttonName == "rb")
		return RIGHT_SHOULDER;
	if (buttonName == "a")
		return A;
	if (buttonName == "b")
		return B;
	if (buttonName == "x")
		return X;
	if (buttonName == "y")
		return Y;
	return RESERVED_0;
}

int GetVirtualKeyCodeFromName(const std::string & keyName)
{
	if (keyName == "backspace") return VK_BACK;
	if (keyName == "delete") return VK_DELETE;
	if (keyName == "tab") return VK_TAB;
	if (keyName == "return") return VK_RETURN;
	if (keyName == "escape") return VK_ESCAPE;
	if (keyName == "space") return VK_SPACE;
	if (keyName == "keypad 0") return VK_NUMPAD0;
	if (keyName == "keypad 1") return VK_NUMPAD1;
	if (keyName == "keypad 2") return VK_NUMPAD2;
	if (keyName == "keypad 3") return VK_NUMPAD3;
	if (keyName == "keypad 4") return VK_NUMPAD4;
	if (keyName == "keypad 5") return VK_NUMPAD5;
	if (keyName == "keypad 6") return VK_NUMPAD6;
	if (keyName == "keypad 7") return VK_NUMPAD7;
	if (keyName == "keypad 8") return VK_NUMPAD8;
	if (keyName == "keypad 9") return VK_NUMPAD9;
	if (keyName == "keypad period") return VK_DECIMAL;
	if (keyName == "keypad divide") return VK_DIVIDE;
	if (keyName == "keypad multiply") return VK_MULTIPLY;
	if (keyName == "keypad minus") return VK_SUBTRACT;
	if (keyName == "keypad plus") return VK_ADD;
	if (keyName == "keypad enter") return VK_RETURN;
	if (keyName == "up") return VK_UP;
	if (keyName == "down") return VK_DOWN;
	if (keyName == "right") return VK_RIGHT;
	if (keyName == "left") return VK_LEFT;
	if (keyName == "insert") return VK_INSERT;
	if (keyName == "home") return VK_HOME;
	if (keyName == "end") return VK_END;
	if (keyName == "page up") return VK_PRIOR;
	if (keyName == "page down") return VK_NEXT;
	if (keyName == "f1") return VK_F1;
	if (keyName == "f2") return VK_F2;
	if (keyName == "f3") return VK_F3;
	if (keyName == "f4") return VK_F4;
	if (keyName == "f5") return VK_F5;
	if (keyName == "f6") return VK_F6;
	if (keyName == "f7") return VK_F7;
	if (keyName == "f8") return VK_F8;
	if (keyName == "f9") return VK_F9;
	if (keyName == "f10") return VK_F10;
	if (keyName == "f11") return VK_F11;
	if (keyName == "f12") return VK_F12;
	if (keyName == "f13") return VK_F13;
	if (keyName == "f14") return VK_F14;
	if (keyName == "f15") return VK_F15;
	if (keyName == "0") return '0';
	if (keyName == "1") return '1';
	if (keyName == "2") return '2';
	if (keyName == "3") return '3';
	if (keyName == "4") return '4';
	if (keyName == "5") return '5';
	if (keyName == "6") return '6';
	if (keyName == "7") return '7';
	if (keyName == "8") return '8';
	if (keyName == "9") return '9';
	if (keyName == "quote") return VK_OEM_7;
	if (keyName == "comma") return VK_OEM_COMMA;
	if (keyName == "minus") return VK_OEM_MINUS;
	if (keyName == "period") return VK_OEM_PERIOD;
	if (keyName == "slash") return VK_OEM_2;
	if (keyName == "semicolon") return VK_OEM_1;
	if (keyName == "equals") return VK_OEM_PLUS;
	if (keyName == "left bracket") return VK_OEM_4;
	if (keyName == "backslash") return VK_OEM_5;
	if (keyName == "right bracket") return VK_OEM_6;
	if (keyName == "back quote") return VK_OEM_3;
	if (keyName == "a") return 'A';
	if (keyName == "b") return 'B';
	if (keyName == "c") return 'C';
	if (keyName == "d") return 'D';
	if (keyName == "e") return 'E';
	if (keyName == "f") return 'F';
	if (keyName == "g") return 'G';
	if (keyName == "h") return 'H';
	if (keyName == "i") return 'I';
	if (keyName == "j") return 'J';
	if (keyName == "k") return 'K';
	if (keyName == "l") return 'L';
	if (keyName == "m") return 'M';
	if (keyName == "n") return 'N';
	if (keyName == "o") return 'O';
	if (keyName == "p") return 'P';
	if (keyName == "q") return 'Q';
	if (keyName == "r") return 'R';
	if (keyName == "s") return 'S';
	if (keyName == "t") return 'T';
	if (keyName == "u") return 'U';
	if (keyName == "v") return 'V';
	if (keyName == "w") return 'W';
	if (keyName == "x") return 'X';
	if (keyName == "y") return 'Y';
	if (keyName == "z") return 'Z';
	if (keyName == "num lock") return VK_NUMLOCK;
	if (keyName == "caps lock") return VK_CAPITAL;
	if (keyName == "scroll lock") return VK_SCROLL;
	if (keyName == "right shift") return VK_SHIFT;
	if (keyName == "left shift") return VK_SHIFT;
	if (keyName == "right control") return VK_CONTROL;
	if (keyName == "left control") return VK_CONTROL;
	if (keyName == "right alt") return VK_MENU;
	if (keyName == "left alt") return VK_MENU;
	if (keyName == "left windows") return VK_LWIN;
	if (keyName == "right windows") return VK_RWIN;
	if (keyName == "menu") return VK_MENU;

	return -1;
}

int GetMouseButtonFromName(const std::string & mouseButtonName)
{
	if (mouseButtonName == "0") return 0;
	if (mouseButtonName == "1") return 1;
	if (mouseButtonName == "2") return 2;
	if (mouseButtonName == "3") return 3;
	if (mouseButtonName == "4") return 4;
	if (mouseButtonName == "5") return 5;
	return -1;
}

DSJoystickAxisCode GetJoystickAxisCodeFromAxisCode(DSAxisCode axisCode)
{
	switch (axisCode) {
	case JoystickLT: return LT;
	case JoystickRT: return RT;
	case JoystickLX: return LX;
	case JoystickLY: return LY;
	case JoystickRX: return RX;
	case JoystickRY: return RY;
	default: return ERROR_AXIS;
	}
}

void DSSInputAxis::BindButtonCallback(const std::string & buttonName, int buttonFuncIndex)
{
	// Determine if the buttonName is from keyboard or joystick or mouse
	if (buttonName.empty())
	{
		// Not assigned, does nothing
	}
	else if (boost::starts_with(buttonName, "mouse"))
	{
		// mouse
		std::vector<std::string> strs;
		split(strs, buttonName, boost::is_any_of(" "));

		int buttonCode = GetMouseButtonFromName(strs[1]);

		if (buttonCode < 0)
		{
			getButton[buttonFuncIndex] = [buttonName]() { LOG_ERROR << "Mouse Button \"" << buttonName << "\" Can Not Be Parsed!"; return false; };
			getButtonDown[buttonFuncIndex] = [buttonName]() { LOG_ERROR << "Mouse Button \"" << buttonName << "\" Can Not Be Parsed!"; return false; };
			getButtonUp[buttonFuncIndex] = [buttonName]() { LOG_ERROR << "Mouse Button \"" << buttonName << "\" Can Not Be Parsed!"; return false; };
		}
		else
		{
			getButton[buttonFuncIndex] = std::bind(&DSFRawInput::GetMouseButton, &inputSystem->rawInput, buttonCode);
			getButtonDown[buttonFuncIndex] = std::bind(&DSFRawInput::GetMouseButtonDown, &inputSystem->rawInput, buttonCode);
			getButtonUp[buttonFuncIndex] = std::bind(&DSFRawInput::GetMouseButtonUp, &inputSystem->rawInput, buttonCode);
		}
	}
	else if (boost::starts_with(buttonName, "joystick"))
	{
		// joystick

		// button name
		std::vector<std::string> strs;
		split(strs, buttonName, boost::is_any_of(" "));

		DSJoystickButtonCode buttonCode = GetJoystickButtonCodeFromName(strs[1]);

		if (buttonCode == RESERVED_0)
		{
			getButton[buttonFuncIndex] = [buttonName]() { LOG_ERROR << "Joystick Button \"" << buttonName << "\" Can Not Be Parsed!"; return false; };
			getButtonDown[buttonFuncIndex] = [buttonName]() { LOG_ERROR << "Joystick Button \"" << buttonName << "\" Can Not Be Parsed!"; return false; };
			getButtonUp[buttonFuncIndex] = [buttonName]() { LOG_ERROR << "Joystick Button \"" << buttonName << "\" Can Not Be Parsed!"; return false; };
		}
		else
		{
			getButton[buttonFuncIndex] = std::bind(&DSFXInput::GetButton, &inputSystem->xInput, buttonCode, joyNum);
			getButtonDown[buttonFuncIndex] = std::bind(&DSFXInput::GetButtonDown, &inputSystem->xInput, buttonCode, joyNum);
			getButtonUp[buttonFuncIndex] = std::bind(&DSFXInput::GetButtonUp, &inputSystem->xInput, buttonCode, joyNum);
		}
	}
	else
	{
		// keyboard
		int keyCode = GetVirtualKeyCodeFromName(buttonName);

		if (keyCode < 0)
		{
			getButton[buttonFuncIndex] = [buttonName]() { LOG_ERROR << "Key \"" << buttonName << "\" Can Not Be Parsed!"; return false; };
			getButtonDown[buttonFuncIndex] = [buttonName]() { LOG_ERROR << "Key \"" << buttonName << "\" Can Not Be Parsed!"; return false; };
			getButtonUp[buttonFuncIndex] = [buttonName]() { LOG_ERROR << "Key \"" << buttonName << "\" Can Not Be Parsed!"; return false; };
		}
		else
		{
			getButton[buttonFuncIndex] = std::bind(&DSFRawInput::GetKey, &inputSystem->rawInput, keyCode);
			getButtonDown[buttonFuncIndex] = std::bind(&DSFRawInput::GetKeyDown, &inputSystem->rawInput, keyCode);
			getButtonUp[buttonFuncIndex] = std::bind(&DSFRawInput::GetKeyUp, &inputSystem->rawInput, keyCode);
		}
	}
}

void DSSInputAxis::BindCallbackFunctions()
{
	if (type == Button)
	{
		if (invert)
		{
			BindButtonCallback(posButton, 2);
			BindButtonCallback(altPosButton, 3);
			BindButtonCallback(negButton, 0);
			BindButtonCallback(altNegButton, 1);
		}
		else
		{
			BindButtonCallback(posButton, 0);
			BindButtonCallback(altPosButton, 1);
			BindButtonCallback(negButton, 2);
			BindButtonCallback(altNegButton, 3);
		}

		getRawAxis = [this]()
		{
			float result = 0;

			if (getButton[0]() || getButton[1]()) result += 1;
			if (getButton[2]() || getButton[3]()) result -= 1;

			return result;
		};
	}
	else if (type == Axis)
	{
		for (int i = 0; i < 3; i += 2)
		{
			getButton[i] = []() { LOG_ERROR << "Can't call GetButton on an axis type input!"; return false; };
			getButtonDown[i] = []() { LOG_ERROR << "Can't call GetButtonDown on an axis type input!"; return false; };
			getButtonUp[i] = []() { LOG_ERROR << "Can't call GetButtonUp on an axis type input!"; return false; };
		}

		DSJoystickAxisCode joystickAxisCode = GetJoystickAxisCodeFromAxisCode(axis);

		getRawAxis = [this, joystickAxisCode]() {
			return inputSystem->xInput.GetRawAxis(joystickAxisCode, joyNum) * (invert ? -1 : 1);
		}; \
	}
	else if (type == Movement)
	{
		for (int i = 0; i < 3; i += 2)
		{
			getButton[i] = []() { LOG_ERROR << "Can't call GetButton on an movement type input!"; return false; };
			getButtonDown[i] = []() { LOG_ERROR << "Can't call GetButtonDown on an movement type input!"; return false; };
			getButtonUp[i] = []() { LOG_ERROR << "Can't call GetButtonUp on an movement type input!"; return false; };
		}

		std::function<LONG()> getMouseDelta;

		switch (axis)
		{
		case MouseX:
			getMouseDelta = std::bind(&DSFRawInput::GetMouseDeltaX, &inputSystem->rawInput);
			break;
		case MouseY:
			getMouseDelta = std::bind(&DSFRawInput::GetMouseDeltaY, &inputSystem->rawInput);
			break;
		case MouseWheel:
			getMouseDelta = [this]()
			{
				return LONG(inputSystem->rawInput.GetMouseWheelDelta());
			};
			break;
		default:
			getMouseDelta = []()
			{
				LOG_ERROR << "Can't get joystick axis with input type movement!";
				return LONG(0);
			};
			break;
		}

		getRawAxis = [this, getMouseDelta]()
		{
			auto mouseDelta = float(getMouseDelta());
			mouseDelta = mouseDelta * sensitivity * (invert ? -1 : 1);
			return mouseDelta;
		};
	}
}

void DSSInputAxis::Update(float deltaTime)
{
	const float rawData = getRawAxis();
	if (type == Axis)
	{
		if (abs(rawData) < dead)
			processedAxisValue = 0;
		else
			processedAxisValue = rawData;
	}
	else if (type == Button)
	{
		if (abs(rawData) <= dead)
		{
			if (processedAxisValue > 0)
			{
				processedAxisValue -= gravity * deltaTime;
				if (processedAxisValue < 0) processedAxisValue = 0;
			}
			else if (processedAxisValue < 0)
			{
				processedAxisValue += gravity * deltaTime;
				if (processedAxisValue > 0) processedAxisValue = 0;
			}
		}
		else
		{
			if (rawData > 0) processedAxisValue += sensitivity * deltaTime;
			else processedAxisValue -= sensitivity * deltaTime;

			if (processedAxisValue > 1.0f) processedAxisValue = 1.0f;
			else if (processedAxisValue < -1.0f) processedAxisValue = -1.0f;
		}
	}
	else if (type == Movement)
	{
		if (abs(rawData) < dead)
			processedAxisValue = 0;
		else
			processedAxisValue = rawData / (deltaTime * 7500.0f);
	}
}

DSSInputAxis::DSSInputAxis(std::string name, std::string posButton, std::string negButton, std::string altPosButton,
	std::string altNegButton, float gravity, float dead, float sensitivity, bool invert, InputType type,
	DSAxisCode axis, int joyNum, DSSInput * system)
	: name(std::move(name))
	, posButton(std::move(posButton))
	, negButton(std::move(negButton))
	, altPosButton(std::move(altPosButton))
	, altNegButton(std::move(altNegButton))
	, gravity(gravity)
	, dead(dead)
	, sensitivity(sensitivity)
	, invert(invert)
	, type(type)
	, axis(axis)
	, joyNum(joyNum)
	, inputSystem(system)
{
	processedAxisValue = 0;

	for (int i = 0; i < 4; ++i)
	{
		getButton[i] = []() { return false; };
		getButtonDown[i] = []() { return false; };
		getButtonUp[i] = []() { return false; };
	}

	BindCallbackFunctions();
}
