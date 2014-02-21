
#include "volymerna/Sound.h"

// internal
#include "volymerna/System.h"
#include "volymerna/File.h"
#include "volymerna/Source.h"
#include "volymerna/Listener.h"

// external

namespace volymerna {

Sound::Sound(const std::shared_ptr<File> &file) :
	file(file)
{
}

void Sound::Recalculate()
{
//	currTime = time;

	float dist = glm::distance(file->source->position, listener->position);
	float spm = (1.0f/System::Get()->speedOfSound);
	timeDiff = spm*dist; // Speed of Sound sec/meter * Distance

	if(file->currTime >= file->totalTime+timeDiff)
	{
		return;
	}

	// play audio at (time - diff)
	float frameVol = file->GetVolume(GetTime());
	volume = (frameVol + (20.0f * std::log10(1.0f/dist)))/frameVol * GetVolume();

	/// @todo Set volume..?
}

void Sound::Update()
{
	file->Seek(GetTime());
	file->PlayChunk();

	if(timer > 0.0f)
	{
		timer -= System::Get()->GetDeltaTime();
	}
	else if(timer <= 0.0f && timer > -1.0f)
	{
		if(timerCmd == "pause")
		{
			// Pause...
		}
		else if(timerCmd == "play")
		{
			// Play...
		}
		else if(timerCmd == "stop")
		{
			// Destroy this sound
		}
		timer = -1.0f;
	}
}

float Sound::GetVolume()
{
	return file->source->volume;
}

void Sound::SetTimer(string cmd)
{
	timer = timeDiff;
	timerCmd = cmd;
}

float Sound::GetTime()
{
	return file->currTime - timeDiff;
}

}
