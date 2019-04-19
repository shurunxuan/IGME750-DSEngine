#pragma once

#include <boost/thread/thread.hpp>
#include "Object.hpp"
#include "Component.hpp"
#include <AudioSource.hpp>

/**
 * @brief Press Space to play audio
 * 
 */
class PressSpaceToPlayAudio :
	public Component
{
public:
	explicit PressSpaceToPlayAudio(Object* owner);
	~PressSpaceToPlayAudio();

	void Start() override;
	void Update(float deltaTime, float totalTime) override;

	AudioSource* source1;
	AudioSource* source2;

private:

};

