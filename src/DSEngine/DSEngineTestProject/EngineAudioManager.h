#pragma once
#pragma once
#include <list>
#include "Component.hpp"
#include "AudioSource.hpp"

/**
 * @brief Move an object with arrow keys, rotate with numpad, change material property with DPad
 *
 */
class EngineAudioManager :
	public Component
{
public:
	explicit EngineAudioManager(Object* owner);
	~EngineAudioManager();

	void Start() override;
	void Update(float deltaTime, float totalTime) override;

	
	AudioSource* startup;
	AudioSource* idle;
	AudioSource* maxRPM;
	AudioSource* highOn;
	AudioSource* midOn;
	AudioSource* lowOn;
private:
	bool isStart;
	float RPM;
	float maxRPMValue;
	float clipValue;
	float optVolume;

	float IdleVolumeCurve(float clipValue);
	float IdlePitchCurve(float clipValue);
	float LowVolumeCurve(float clipValue);
	float LowPItchCurve(float clipValue);
	float midVolumeCurve(float clipValue);
	float midPitchCurve(float clipValue);
	float highVolumeCurve(float clipValue);
	float highPitchCurve(float clipValue);
};

