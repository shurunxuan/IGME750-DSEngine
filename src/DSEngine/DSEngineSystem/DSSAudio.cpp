#include "DSSAudio.h"
#include "DSFLogging.h"


DSSAudio::DSSAudio()
{
}


DSSAudio::~DSSAudio()
{
}

void DSSAudio::Init()
{
	xAudio2.Init();
	LOG_TRACE << "DS Engine Audio System Initialized!";
}
