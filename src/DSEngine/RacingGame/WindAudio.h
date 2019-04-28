#pragma once
#include "Component.hpp"
#include "AudioSource.hpp"

class WindAudio :
	public Component
{
public:
	explicit WindAudio(Object* owner);
	~WindAudio();

	AudioSource* windAudioSource;
	AudioListener* listener;

	void Start() override;
	void Update(float deltaTime, float totalTime) override;

private:
};

