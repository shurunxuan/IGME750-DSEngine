#include "DSSAudio.h"
#include "DSFLogging.h"
#include "DSEngineApp.h"
#include "AudioSource.hpp"
#include "AudioListener.hpp"

DSSAudio* SAudio = nullptr;

DSSAudio::DSSAudio()
{
	SAudio = this;
}


DSSAudio::~DSSAudio()
{
}

void DSSAudio::Init()
{
	xAudio2.Init();
	LOG_TRACE << "DS Engine Audio System Initialized!";
}

void DSSAudio::Update()
{
	std::list<AudioSource*> audioSourcesInScene;

	// Get All AudioSources
	audioSourcesInScene.merge(App->CurrentActiveScene()->mainCamera->GetComponents<AudioSource>());
	for (Object* object : App->CurrentActiveScene()->allObjects)
	{
		audioSourcesInScene.merge(object->GetComponents<AudioSource>());
	}

	AudioListener* audioListener = nullptr;
	audioListener = App->CurrentActiveScene()->mainCamera->GetComponent<AudioListener>();
	if (audioListener == nullptr)
	{
		for (Object* object : App->CurrentActiveScene()->allObjects)
		{
			AudioListener* listener = object->GetComponent<AudioListener>();
			if (listener == nullptr) continue;
			audioListener = listener;
			break;
		}
	}
	if (audioListener == nullptr)
	{
		LOG_ERROR << "No AudioListener in scene!";
		return;
	}
	for (AudioSource* source : audioSourcesInScene)
	{
		// Don't calculate if source is not 3D
		if (!source->Is3D) continue;
		if (!source->Playing()) continue;
		xAudio2.AudioCalculate3D(audioListener, source);
	}
}
