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
	LOG_DEBUG << speedM.m128_f32[0];
}
