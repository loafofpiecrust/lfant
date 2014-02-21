
#pragma once
#include "lfant/stdafx.h"

// internal

// external
#include <string>
#include <deque>

namespace volymerna {

class Sound;
class Source;

class File
{
public:
	File(Source* src) : source(src) {}
//	~File() {}

//	virtual void Load(std::string path) = 0;
	virtual void PlayChunk() = 0;
	virtual float GetVolume(float time) = 0;
	virtual void Seek(float time) = 0;

	void Update();

//protected:

	string name = "";
	float totalTime = 1.0f;
	std::deque<Sound*> sounds;
	Source* source = nullptr;
	float currTime = 0.0f;

private:
};

}
