#pragma once

#include <boost/thread/thread.hpp>
#include "Object.hpp"
#include "Component.hpp"

class PressSpaceToPlayAudio :
	public Component
{
public:
	explicit PressSpaceToPlayAudio(Object* owner);
	~PressSpaceToPlayAudio();

	void Start() override;
	void Update(float deltaTime, float totalTime) override;

private:
	bool isPlaying;
	/**
	 * @brief Temporary audio playback thread for testing
	 */
	boost::thread playbackThread;
};

