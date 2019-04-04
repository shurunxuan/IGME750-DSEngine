#include <DirectXMath.h>
#include "InputManager.h"
#include "Object.hpp"
#include "DSSInput.h"


InputManager::InputManager(Object* owner)
	: Component(owner)
{
}


InputManager::~InputManager()
{
}

void InputManager::Start()
{
}

void InputManager::Update(float deltaTime, float totalTime)
{
	if (SInput->GetButton("RightButton"))
	{

	}
}
