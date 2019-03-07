#include "DSFRawInput.h"
#include "DSFLogging.h"

DSFRawInput* FRawInput = nullptr;

DSFRawInput::DSFRawInput()
{
	FRawInput = this;
}


DSFRawInput::~DSFRawInput()
{
}

void DSFRawInput::Init()
{

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
	// TODO: Do something here
}

void DSFRawInput::OnKeyboardInput(RAWKEYBOARD raw)
{
	//raw.MakeCode,
//	raw.Flags,
//	raw.Reserved,
//	raw.ExtraInformation,
//	raw.Message,
//	raw.VKey
	keyboardInput = raw;
}

void DSFRawInput::OnMouseInput(RAWMOUSE raw)
{
	//raw.usFlags,
//	raw.ulButtons,
//	raw.usButtonFlags,
//	raw.usButtonData,
//	raw.ulRawButtons,
//	raw.lLastX,
//	raw.lLastY,
//	raw.ulExtraInformation
	mouseInput = raw;
}
