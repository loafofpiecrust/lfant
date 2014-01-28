
#include <volymerna/Source.h>

// internal
#include <volymerna/System.h>

// external

namespace volymerna {

/*******
 *	Sound
 ******/
void Sound::Play()
{
	playing = true;
}

void Sound::Pause()
{
	playing = false;
}

void Sound::Stop()
{
	Pause();
	time = 0.0f;
}


/*******
 *	Source
 ******/
Source::Source()
{

}

Source::~Source()
{
	for(auto& snd : sounds)
	{
		if(snd->file)
		{
			delete snd->file;
		}
	}
}

void Source::LoadFile(string name, File* input)
{
	Sound* snd = new Sound(name, input, this);
	files.push_back(snd);
	if(!current)
	{
		current = snd;
	}
}

void Source::Update()
{
	for(auto& snd : sounds)
	{
		if(snd->playing && snd->file)
		{
			// progress time
			snd->time += game->time->deltaTime;

			// check if the sound has travelled the farthest it can
			float spm = (1.0f/System::Get()->speedOfSound); // sec/meter
			if(snd->time >= snd->file->totalTime+(spm*radius))
			{
				snd->Stop();
			}
		}
	}
}

void Sound::PlayCurrentFrame(float dist)
{
	if(!playing) return;

	float spm = (1.0f/System::Get()->speedOfSound);
	float diff = spm*dist; // Speed of Sound sec/meter * Distance
	
	if(time >= file->totalTime+diff)
	{
		return;
	}

	// play audio at (time - diff)
	float frameVol = file->GetVolume(time - diff);
	float volume = (frameVol + (20.0f * log(10, 1.0f/dist)))/frameVol * source->volume;
	file->Play(time - diff, volume);
}

Sound* Source::GetCurrent()
{
	return current;
}

Sound* Source::GetSound(string name)
{
	for(auto& snd : sounds)
	{
		if(snd->name == name)
		{
			return snd;
		}
	}
}

}