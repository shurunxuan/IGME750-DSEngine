#include <DirectXMath.h>
#include "InputManager.h"
#include "Object.hpp"
#include "DSSInput.h"
#include "BrickDeck.h"
#include "PlayerManager.h"
#include "BoardManager.h"
#include "TaskCardGenerator.h"


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
	if (SInput->GetButtonDown("1"))
	{
		if (GameManager::getInstance()->currentGameState == Idle)
		{
			GameManager::getInstance()->currentGameState = BrickSelected;
			GameManager::getInstance()->selectedBrick = PlayerManager::getInstance()->bricksInHand[0];
			DirectX::XMVECTOR position = GameManager::getInstance()->selectedBrick->object->transform->GetLocalTranslation();
			position = DirectX::XMVectorAdd(position, DirectX::XMVectorSet(0.0f, 0.5f, 0.0f, 0.0f));
			GameManager::getInstance()->selectedBrick->object->transform->SetLocalTranslation(position);
		}
		else if (GameManager::getInstance()->currentGameState == BrickSelected) {
			if (BoardManager::getInstance()->PlaceBrickAt(GameManager::getInstance()->selectedBrick, BoardManager::getInstance()->boardColums[0]))
			{
				GameManager::getInstance()->currentGameState = Idle;
			}
		}
	}
	if (SInput->GetButtonDown("2"))
	{
		if (GameManager::getInstance()->currentGameState == Idle)
		{
			GameManager::getInstance()->currentGameState = BrickSelected;
			GameManager::getInstance()->selectedBrick = PlayerManager::getInstance()->bricksInHand[1];
			DirectX::XMVECTOR position = GameManager::getInstance()->selectedBrick->object->transform->GetLocalTranslation();
			position = DirectX::XMVectorAdd(position, DirectX::XMVectorSet(0.0f, 0.5f, 0.0f, 0.0f));
			GameManager::getInstance()->selectedBrick->object->transform->SetLocalTranslation(position);
		}
		else if (GameManager::getInstance()->currentGameState == BrickSelected) {
			if (BoardManager::getInstance()->PlaceBrickAt(GameManager::getInstance()->selectedBrick, BoardManager::getInstance()->boardColums[1]))
			{
				GameManager::getInstance()->currentGameState = Idle;
			}
		}
	}
	if (SInput->GetButtonDown("3"))
	{
		if (GameManager::getInstance()->currentGameState == Idle)
		{
			GameManager::getInstance()->currentGameState = BrickSelected;
			GameManager::getInstance()->selectedBrick = PlayerManager::getInstance()->bricksInHand[2];
			DirectX::XMVECTOR position = GameManager::getInstance()->selectedBrick->object->transform->GetLocalTranslation();
			position = DirectX::XMVectorAdd(position, DirectX::XMVectorSet(0.0f, 0.5f, 0.0f, 0.0f));
			GameManager::getInstance()->selectedBrick->object->transform->SetLocalTranslation(position);
		}
		else if (GameManager::getInstance()->currentGameState == BrickSelected) {
			if (BoardManager::getInstance()->PlaceBrickAt(GameManager::getInstance()->selectedBrick, BoardManager::getInstance()->boardColums[2]))
			{
				GameManager::getInstance()->currentGameState = Idle;
			}
		}
	}
	if (SInput->GetButtonDown("ESC"))
	{
		if (GameManager::getInstance()->currentGameState == BrickSelected) {
			DirectX::XMVECTOR position = GameManager::getInstance()->selectedBrick->object->transform->GetLocalTranslation();
			position = DirectX::XMVectorSubtract(position, DirectX::XMVectorSet(0.0f, 0.5f, 0.0f, 0.0f));
			GameManager::getInstance()->selectedBrick->object->transform->SetLocalTranslation(position);
			GameManager::getInstance()->currentGameState = Idle;
		}
	}
	if (SInput->GetButtonDown("DrawCard")) {
		if (GameManager::getInstance()->currentGameState == BrickSelected) {
			DirectX::XMVECTOR position = GameManager::getInstance()->selectedBrick->object->transform->GetLocalTranslation();
			position = DirectX::XMVectorSubtract(position, DirectX::XMVectorSet(0.0f, 0.5f, 0.0f, 0.0f));
			GameManager::getInstance()->selectedBrick->object->transform->SetLocalTranslation(position);
			GameManager::getInstance()->currentGameState = Idle;
		}

		if (GameManager::getInstance()->ContinousDrawTask == true)
		{
			PlayerManager::getInstance()->DrawTaskCard();
			GameManager::getInstance()->ContinousDrawTask = false;
		}
		GameManager::getInstance()->Update(Idle);

	}
}
