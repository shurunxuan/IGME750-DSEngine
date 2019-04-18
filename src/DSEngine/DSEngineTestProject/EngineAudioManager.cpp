#include "EngineAudioManager.h"

EngineAudioManager::EngineAudioManager(Object * owner)
	: Component(owner)
{
	
}

EngineAudioManager::~EngineAudioManager()
{
}

void EngineAudioManager::Start()
{
	isStart = false;
	maxRPMValue = 6.0f;
	optVolume = 0.01f;
	
}

void EngineAudioManager::Update(float deltaTime, float totalTime)
{
	RPM = sqrt((object->GetComponent<RigidBody>()->GetVelocity().x)*(object->GetComponent<RigidBody>()->GetVelocity().x) +
		(object->GetComponent<RigidBody>()->GetVelocity().y)*(object->GetComponent<RigidBody>()->GetVelocity().y) +
		(object->GetComponent<RigidBody>()->GetVelocity().z)*(object->GetComponent<RigidBody>()->GetVelocity().z));
	LOG_DEBUG << IdleVolumeCurve(clipValue);

	clipValue = RPM / maxRPMValue;
	if (!isStart) {
		startup->Play();
		isStart = true;
	}
	
	//
	if (isStart&&(!startup->Playing())) {
		
		idle->SetVolume(IdleVolumeCurve(clipValue));
		idle->SetFrequencyRatio(IdlePitchCurve(clipValue));
		lowOn->SetVolume(LowVolumeCurve(clipValue));
		lowOn->SetFrequencyRatio(LowPItchCurve(clipValue));
		midOn->SetVolume(midVolumeCurve(clipValue));
		midOn->SetFrequencyRatio(midPitchCurve(clipValue));
		highOn->SetVolume(highVolumeCurve(clipValue));
		highOn->SetFrequencyRatio(highPitchCurve(clipValue));
		
		if (idle->GetVolume() > optVolume) {
			idle->Play();
		}
		else {
			idle->Pause();
		}
		if (lowOn->GetVolume() > optVolume) {
			lowOn->Play();
		}
		else {
			lowOn->Pause();
		}
		if (midOn->GetVolume() > optVolume) {
			midOn->Play();
		}
		else {
			midOn->Pause();//why stop can't work?
		}
		if (highOn->GetVolume() > optVolume) {
			highOn->Play();
		}
		else {
			highOn->Pause();
		}


		
			
			
		
	}
		
	
}

float EngineAudioManager::IdleVolumeCurve(float clipValue)
{
	
	return -4.875*clipValue*clipValue + 0.9625*clipValue + 0.5640625;
}

float EngineAudioManager::IdlePitchCurve(float clipValue)
{
	return 4.0f*clipValue+0.45f;
}

float EngineAudioManager::LowVolumeCurve(float clipValue)
{
	return -20.416667f*clipValue*clipValue + 7.349999999999937f*clipValue - 0.061458333333332726f;
}

float EngineAudioManager::LowPItchCurve(float clipValue)
{
	return 3.0f*clipValue;
}

float EngineAudioManager::midVolumeCurve(float clipValue)
{
	return -7.905982905983102*clipValue*clipValue + 4.03205128205136*clipValue - 0.029529914529915293;
}

float EngineAudioManager::midPitchCurve(float clipValue)
{
	return 2.0f*clipValue;
}

float EngineAudioManager::highVolumeCurve(float clipValue)
{
	return -1.2083333333333353f*clipValue*clipValue + 2.312500000000001f*clipValue - 0.40416666666666673f;
}

float EngineAudioManager::highPitchCurve(float clipValue)
{
	return 3.0f*clipValue;
}
