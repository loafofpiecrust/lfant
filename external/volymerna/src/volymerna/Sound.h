
#pragma once
#include <lfant/stdafx.h>

// internal

// external
#include <fstream>
#include <memory>

namespace volymerna {

class File;
class Source;
class Listener;

class Sound
{
public:
	Sound(const std::shared_ptr<File>& file);

	void Recalculate();

	void Update();

	// Multiplies the volumes from source and file
	float GetVolume();

	void SetTimer(string cmd);


protected:

private:
	float GetTime();

//	float currTime = 0.0f;
//	File file;
	std::shared_ptr<File> file;
//	SoundManager* manager = nullptr;
	Listener* listener = nullptr;

	float timeDiff = 0.0f;
	float currTime = 0.0f;
	float volume = 1.0f;

	float timer = -1.0f;
	string timerCmd = "";
};

}
