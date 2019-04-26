#include "WindAudio.h"
#include "AudioListener.hpp"

WindAudio::WindAudio(Object* owner)
	: Component(owner)
{
	windAudioSource = nullptr;
	listener = nullptr;
}

WindAudio::~WindAudio()
{
}

void WindAudio::Start()
{
}

void WindAudio::Update(float deltaTime, float totalTime)
{
	DirectX::XMFLOAT3 speed = listener->GetSpeed();
	DirectX::XMVECTOR speedV = DirectX::XMLoadFloat3(&speed);
	DirectX::XMVECTOR speedM = DirectX::XMVector3Length(speedV);
	float listenerSpeed = speedM.m128_f32[0];

	DirectX::XMVECTOR distance = DirectX::XMVectorSubtract(windAudioSource->object->transform->GetGlobalTranslation(), listener->object->transform->GetGlobalTranslation());
	DirectX::XMVECTOR distanceM = DirectX::XMVector3Length(distance);
	float dis = distanceM.m128_f32[0];

	windAudioSource->SetVolume(listenerSpeed / dis / 2.0f);
}
