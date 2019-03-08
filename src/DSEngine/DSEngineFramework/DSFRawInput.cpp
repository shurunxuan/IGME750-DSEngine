#include "DSFRawInput.h"
#include "DSFLogging.h"

DSFRawInput* FRawInput = nullptr;

#define caseStringify(x) case x: return std::string(#x)

DSFRawInput::DSFRawInput()
{
	FRawInput = this;
	hWnd = nullptr;
}


DSFRawInput::~DSFRawInput()
{
}

void DSFRawInput::Init(HWND hWnd)
{
	this->hWnd = hWnd;
	// Register raw input devices
	RAWINPUTDEVICE Rid[2];

	Rid[0].usUsagePage = 0x01;
	Rid[0].usUsage = 0x02;
	Rid[0].dwFlags = 0;// RIDEV_NOLEGACY;   // adds HID mouse and also ignores legacy mouse messages
	Rid[0].hwndTarget = nullptr;

	Rid[1].usUsagePage = 0x01;
	Rid[1].usUsage = 0x06;
	Rid[1].dwFlags = 0;// RIDEV_NOLEGACY;   // adds HID keyboard and also ignores legacy keyboard messages
	Rid[1].hwndTarget = nullptr;

	if (RegisterRawInputDevices(Rid, 2, sizeof(RAWINPUTDEVICE)) == FALSE) {
		//registration failed. Call GetLastError for the cause of the error
		DWORD error = GetLastError();
		LOG_ERROR << "Failed to register mouse & keyboard as raw input devices.";
	}

	LOG_TRACE << "DS Engine Framework for Raw Input Initialized!";
}

void DSFRawInput::Update()
{
	for (int i = 0; i < 256; ++i)
	{
		keyDownStates[i] =
			keyStates[i] &&
			!lastKeyStates[i];

		keyUpStates[i] =
			lastKeyStates[i] &&
			!keyStates[i];

		lastKeyStates[i] = keyStates[i];
	}

	for (int i = 0; i < 5; ++i)
	{
		mouseButtonDownStates[i] =
			mouseButtonStates[i] &&
			!lastMouseButtonStates[i];

		mouseButtonUpStates[i] =
			lastMouseButtonStates[i] &&
			!mouseButtonStates[i];

		lastMouseButtonStates[i] = mouseButtonStates[i];
	}
}

void DSFRawInput::OnKeyboardInput(RAWKEYBOARD raw)
{
	if (raw.Flags & RI_KEY_BREAK)
	{
		keyStates[raw.VKey] = false;
	}
	else // RI_KEY_MAKE
	{
		keyStates[raw.VKey] = true;
	}
}

void DSFRawInput::OnMouseInput(RAWMOUSE raw)
{
	if (raw.usFlags == MOUSE_MOVE_RELATIVE)
	{
		mouseDeltaX = raw.lLastX;
		mouseDeltaY = raw.lLastY;
	}

	if (raw.usButtonFlags == RI_MOUSE_WHEEL)
	{
		mouseDeltaWheel = raw.usButtonData;
	}
	else
	{
		mouseDeltaWheel = 0;

		switch (raw.usButtonFlags)
		{
		case RI_MOUSE_BUTTON_1_DOWN:
			mouseButtonStates[0] = true;
			break;
		case RI_MOUSE_BUTTON_1_UP:
			mouseButtonStates[0] = false;
			break;
		case RI_MOUSE_BUTTON_2_DOWN:
			mouseButtonStates[1] = true;
			break;
		case RI_MOUSE_BUTTON_2_UP:
			mouseButtonStates[1] = false;
			break;
		case RI_MOUSE_BUTTON_3_DOWN:
			mouseButtonStates[2] = true;
			break;
		case RI_MOUSE_BUTTON_3_UP:
			mouseButtonStates[2] = false;
			break;
		case RI_MOUSE_BUTTON_4_DOWN:
			mouseButtonStates[3] = true;
			break;
		case RI_MOUSE_BUTTON_4_UP:
			mouseButtonStates[3] = false;
			break;
		case RI_MOUSE_BUTTON_5_DOWN:
			mouseButtonStates[4] = true;
			break;
		case RI_MOUSE_BUTTON_5_UP:
			mouseButtonStates[4] = false;
			break;
		default: break;
		}
	}
}

bool DSFRawInput::GetKey(int VKey)
{
	return keyStates[VKey];
}

bool DSFRawInput::GetKeyDown(int VKey)
{
	return keyDownStates[VKey];
}

bool DSFRawInput::GetKeyUp(int VKey)
{
	return keyUpStates[VKey];
}

bool DSFRawInput::GetMouseButton(int button)
{
	return mouseButtonStates[button];
}

bool DSFRawInput::GetMouseButtonDown(int button)
{
	return mouseButtonDownStates[button];
}

bool DSFRawInput::GetMouseButtonUp(int button)
{
	return mouseButtonUpStates[button];
}

LONG DSFRawInput::GetMousePosX() const
{
	// Get current mouse position
	POINT p;
	if (GetCursorPos(&p))
	{
		if (ScreenToClient(hWnd, &p))
		{
			return p.x;
		}
	}

	return 0;
}

LONG DSFRawInput::GetMousePosY() const
{
	// Get current mouse position
	POINT p;
	if (GetCursorPos(&p))
	{
		if (ScreenToClient(hWnd, &p))
		{
			return p.y;
		}
	}

	return 0;
}

LONG DSFRawInput::GetMouseDeltaX() const
{
	return mouseDeltaX;
}

LONG DSFRawInput::GetMouseDeltaY() const
{
	return mouseDeltaY;
}

USHORT DSFRawInput::GetMouseWheelDelta() const
{
	return mouseDeltaWheel;
}

std::string DSFRawInput::GetKeyName(int VKey)
{
	char c[2] = { 0 };
	if (VKey >= '0' && VKey <= '9') { c[0] = char(VKey); return std::string(c); }
	if (VKey >= 'A' && VKey <= 'Z') { c[0] = char(VKey); return std::string(c); }
	switch (VKey) {
		// VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
		// 0x40 : unassigned
		// VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
		caseStringify(VK_LBUTTON);
		caseStringify(VK_RBUTTON);
		caseStringify(VK_CANCEL);
		caseStringify(VK_MBUTTON);  // NOT contiguous with L & RBUTTON
		caseStringify(VK_XBUTTON1); // NOT contiguous with L & RBUTTON
		caseStringify(VK_XBUTTON2); // NOT contiguous with L & RBUTTON
		caseStringify(VK_BACK);
		caseStringify(VK_TAB);
		caseStringify(VK_CLEAR);
		caseStringify(VK_RETURN);
		caseStringify(VK_SHIFT);
		caseStringify(VK_CONTROL);
		caseStringify(VK_MENU);
		caseStringify(VK_PAUSE);
		caseStringify(VK_CAPITAL);
		caseStringify(VK_KANA);
		caseStringify(VK_JUNJA);
		caseStringify(VK_FINAL);
		caseStringify(VK_KANJI);
		caseStringify(VK_ESCAPE);
		caseStringify(VK_CONVERT);
		caseStringify(VK_NONCONVERT);
		caseStringify(VK_ACCEPT);
		caseStringify(VK_MODECHANGE);
		caseStringify(VK_SPACE);
		caseStringify(VK_PRIOR);
		caseStringify(VK_NEXT);
		caseStringify(VK_END);
		caseStringify(VK_HOME);
		caseStringify(VK_LEFT);
		caseStringify(VK_UP);
		caseStringify(VK_RIGHT);
		caseStringify(VK_DOWN);
		caseStringify(VK_SELECT);
		caseStringify(VK_PRINT);
		caseStringify(VK_EXECUTE);
		caseStringify(VK_SNAPSHOT);
		caseStringify(VK_INSERT);
		caseStringify(VK_DELETE);
		caseStringify(VK_HELP);
		caseStringify(VK_LWIN);
		caseStringify(VK_RWIN);
		caseStringify(VK_APPS);
		caseStringify(VK_SLEEP);
		caseStringify(VK_NUMPAD0);
		caseStringify(VK_NUMPAD1);
		caseStringify(VK_NUMPAD2);
		caseStringify(VK_NUMPAD3);
		caseStringify(VK_NUMPAD4);
		caseStringify(VK_NUMPAD5);
		caseStringify(VK_NUMPAD6);
		caseStringify(VK_NUMPAD7);
		caseStringify(VK_NUMPAD8);
		caseStringify(VK_NUMPAD9);
		caseStringify(VK_MULTIPLY);
		caseStringify(VK_ADD);
		caseStringify(VK_SEPARATOR);
		caseStringify(VK_SUBTRACT);
		caseStringify(VK_DECIMAL);
		caseStringify(VK_DIVIDE);
		caseStringify(VK_F1);
		caseStringify(VK_F2);
		caseStringify(VK_F3);
		caseStringify(VK_F4);
		caseStringify(VK_F5);
		caseStringify(VK_F6);
		caseStringify(VK_F7);
		caseStringify(VK_F8);
		caseStringify(VK_F9);
		caseStringify(VK_F10);
		caseStringify(VK_F11);
		caseStringify(VK_F12);
		caseStringify(VK_F13);
		caseStringify(VK_F14);
		caseStringify(VK_F15);
		caseStringify(VK_F16);
		caseStringify(VK_F17);
		caseStringify(VK_F18);
		caseStringify(VK_F19);
		caseStringify(VK_F20);
		caseStringify(VK_F21);
		caseStringify(VK_F22);
		caseStringify(VK_F23);
		caseStringify(VK_F24);
		caseStringify(VK_NUMLOCK);
		caseStringify(VK_SCROLL);
		caseStringify(VK_OEM_NEC_EQUAL);  // '=' key on numpad
		caseStringify(VK_OEM_FJ_MASSHOU); // 'Unregister word' key
		caseStringify(VK_OEM_FJ_TOUROKU); // 'Register word' key
		caseStringify(VK_OEM_FJ_LOYA);    // 'Left OYAYUBI' key
		caseStringify(VK_OEM_FJ_ROYA);    // 'Right OYAYUBI' key
		caseStringify(VK_LSHIFT);
		caseStringify(VK_RSHIFT);
		caseStringify(VK_LCONTROL);
		caseStringify(VK_RCONTROL);
		caseStringify(VK_LMENU);
		caseStringify(VK_RMENU);
		caseStringify(VK_BROWSER_BACK);
		caseStringify(VK_BROWSER_FORWARD);
		caseStringify(VK_BROWSER_REFRESH);
		caseStringify(VK_BROWSER_STOP);
		caseStringify(VK_BROWSER_SEARCH);
		caseStringify(VK_BROWSER_FAVORITES);
		caseStringify(VK_BROWSER_HOME);
		caseStringify(VK_VOLUME_MUTE);
		caseStringify(VK_VOLUME_DOWN);
		caseStringify(VK_VOLUME_UP);
		caseStringify(VK_MEDIA_NEXT_TRACK);
		caseStringify(VK_MEDIA_PREV_TRACK);
		caseStringify(VK_MEDIA_STOP);
		caseStringify(VK_MEDIA_PLAY_PAUSE);
		caseStringify(VK_LAUNCH_MAIL);
		caseStringify(VK_LAUNCH_MEDIA_SELECT);
		caseStringify(VK_LAUNCH_APP1);
		caseStringify(VK_LAUNCH_APP2);
		caseStringify(VK_OEM_1);      // ';:' for US
		caseStringify(VK_OEM_PLUS);   // '+' any country
		caseStringify(VK_OEM_COMMA);  // ',' any country
		caseStringify(VK_OEM_MINUS);  // '-' any country
		caseStringify(VK_OEM_PERIOD); // '.' any country
		caseStringify(VK_OEM_2);  // '/?' for US
		caseStringify(VK_OEM_3);  // '`~' for US
		caseStringify(VK_OEM_4);  //  '[{' for US
		caseStringify(VK_OEM_5);  //  '\|' for US
		caseStringify(VK_OEM_6);  //  ']}' for US
		caseStringify(VK_OEM_7);  //  ''"' for US
		caseStringify(VK_OEM_8);
		caseStringify(VK_OEM_AX);   //  'AX' key on Japanese AX kbd
		caseStringify(VK_OEM_102);  //  "<>" or "\|" on RT 102-key kbd.
		caseStringify(VK_ICO_HELP); //  Help key on ICO
		caseStringify(VK_ICO_00);   //  00 key on ICO
		caseStringify(VK_PROCESSKEY);
		caseStringify(VK_ICO_CLEAR);
		caseStringify(VK_PACKET);
	default: return "ERROR KEY";
	}
}