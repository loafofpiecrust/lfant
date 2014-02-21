
#include <volymerna/System.h>

// internal

// external

namespace volymerna {

WindowsSystem::WindowsSystem()
{
	XAudio2Create(&engine, 0, XAUDIO2_DEFAULT_PROCESSOR);
	engine->CreateMasteringVoice(&master);
}

}