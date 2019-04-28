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

/**
 * @brief Audio Listener component
 * 
 * There should be one and only one AudioListener component attached to an object. 
 * An AudioListener is automatically attached to the camera when it's added to the scene.
 * 
 */
class AudioListener final
	: public Component
{
private:
	friend class DSFXAudio2;
	/**
	 * @brief The X3DAudio Listener struct
	 * 
	 */
	X3DAUDIO_LISTENER x3dListener;

	/**
	 * @brief The global position of last frame
	 * 
	 */
	DirectX::XMFLOAT3 lastPosition;
public:
	/**
	 * @brief Construct a new Audio Listener object
	 * 
	 * @param owner The object that the component is attached to
	 */
	AudioListener(Object* owner);
	/**
	 * @brief Destroy the Audio Listener object
	 * 
	 */
	~AudioListener();
	/**
	 * @brief Assign the lastPosition as current global position
	 * 
	 */
	void Start() override;
	/**
	 * @brief Update the current position, orientation and velocity of the X3DAudio Listener
	 * 
	 * @param deltaTime The time that a frame costs
	 * @param totalTime The total time from the beginning of the application
	 */
	void Update(float deltaTime, float totalTime) override;

	/**
	 * @brief Get the speed of the listener
	 * 
	 * @return DirectX::XMFLOAT3 The speed of the listener
	 */
	DirectX::XMFLOAT3 GetSpeed();
};

inline AudioListener::AudioListener(Object* owner)
	: Component(owner), lastPosition()
{
	ZeroMemory(&x3dListener, sizeof(X3DAUDIO_LISTENER));
}

inline AudioListener::~AudioListener()
{
}

inline void AudioListener::Start()
{
	DirectX::XMStoreFloat3(&lastPosition, object->transform->GetGlobalTranslation());
}

inline void AudioListener::Update(float deltaTime, float totalTime)
{
	const DirectX::XMVECTOR currentPosition = object->transform->GetGlobalTranslation();
	DirectX::XMVECTOR audioVelocityVec = DirectX::XMVectorSubtract(currentPosition, DirectX::XMLoadFloat3(&lastPosition));
	audioVelocityVec = DirectX::XMVectorScale(audioVelocityVec, 1.0f / deltaTime);
	DirectX::XMStoreFloat3(&lastPosition, currentPosition);
	DirectX::XMStoreFloat3(&x3dListener.OrientFront, object->transform->Forward());
	DirectX::XMStoreFloat3(&x3dListener.OrientTop, object->transform->Up());
	DirectX::XMStoreFloat3(&x3dListener.Position, currentPosition);
	DirectX::XMStoreFloat3(&x3dListener.Velocity, audioVelocityVec);
}

inline DirectX::XMFLOAT3 AudioListener::GetSpeed()
{
	return x3dListener.Velocity;
}
