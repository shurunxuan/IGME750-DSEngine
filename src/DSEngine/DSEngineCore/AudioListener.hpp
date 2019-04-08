/**
 * @file AudioListener.hpp
 * @author Victor Shu
 * @brief This file contains the class that represents
 * the audio x3dListener
 *
 * @version 0.1
 * @date 2019/04/08
 *
 * @copyright Copyright (c) 2019
 *
 */

#pragma once

#include <string>
#include <boost/thread.hpp>
#include "DSFXAudio2.h"
#include "DSFLogging.h"
#include "Component.hpp"
#include "Object.hpp"

class AudioListener
	: public Component
{
private:
	friend class DSFXAudio2;
	X3DAUDIO_LISTENER x3dListener;

	DirectX::XMVECTOR audioVelocity;
	DirectX::XMVECTOR lastPosition;
public:
	AudioListener(Object* owner);
	~AudioListener();
	void Start() override;
	void Update(float deltaTime, float totalTime) override;
};

inline AudioListener::AudioListener(Object* owner)
	: Component(owner)
{
	ZeroMemory(&x3dListener, sizeof(X3DAUDIO_LISTENER));
}

inline AudioListener::~AudioListener()
{
}

inline void AudioListener::Start()
{
	audioVelocity = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	lastPosition = object->transform->GetGlobalTranslation();
}

inline void AudioListener::Update(float deltaTime, float totalTime)
{
	const DirectX::XMVECTOR currentPosition = object->transform->GetGlobalTranslation();
	audioVelocity = DirectX::XMVectorSubtract(currentPosition, lastPosition);
	audioVelocity = DirectX::XMVectorScale(audioVelocity, 1.0f / deltaTime);
	lastPosition = currentPosition;
	DirectX::XMStoreFloat3(&x3dListener.OrientFront, object->transform->Forward());
	DirectX::XMStoreFloat3(&x3dListener.OrientTop, object->transform->Up());
	DirectX::XMStoreFloat3(&x3dListener.Position, currentPosition);
	DirectX::XMStoreFloat3(&x3dListener.Velocity, audioVelocity);
}
