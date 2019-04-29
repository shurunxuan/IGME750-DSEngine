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
	maxRPMValue = 85.0f;
	optVolume = 0.01f;

}

void EngineAudioManager::Update(float deltaTime, float totalTime)
{
	DirectX::XMFLOAT3 velocity = object->GetComponent<RigidBody>()->GetVelocity();
	RPM = sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
	//LOG_DEBUG << RPM;
	//LOG_DEBUG << IdleVolumeCurve(clipValue);

	clipValue = RPM / maxRPMValue;
	if (!isStart) {
		startup->Play();
		isStart = true;
	}

	//
	if (isStart/* && (!startup->Playing())*/) {

		idle->SetVolume(IdleVolumeCurve(clipValue));
		idle->SetFrequencyRatio(IdlePitchCurve(clipValue));
		lowOn->SetVolume(LowVolumeCurve(clipValue) / 2.0f);
		lowOn->SetFrequencyRatio(LowPItchCurve(clipValue));
		midOn->SetVolume(midVolumeCurve(clipValue) / 2.0f);
		midOn->SetFrequencyRatio(midPitchCurve(clipValue));
		highOn->SetVolume(highVolumeCurve(clipValue) / 2.0f);
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

	return -4.875f*clipValue*clipValue + 0.9625f*clipValue + 0.5640f;
}

float EngineAudioManager::IdlePitchCurve(float clipValue)
{
	return 4.0f*clipValue + 0.45f;
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
	return -7.905982905983102f*clipValue*clipValue + 4.03205128205136f*clipValue - 0.029529914529915293f;
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
