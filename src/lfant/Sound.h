
#pragma once
#include <lfant/stdafx.h>

// internal
#include <lfant/ptr.h>

// external
#include <vector>
#include <fstream>

namespace lfant {

class AudioSource;
class File;

class Sound
{
	friend class AudioSource;
public:
	Sound(AudioSource* src, const std::shared_ptr<File>& file);
	~Sound();

	class Resource
	{
	public:
	};

	class Buffer : public Resource
	{
	public:
		uint32 id = 0;
	};

	class Stream : public Resource
	{
	public:
		Stream(uint32& source, string path);

		uint32 bufferCount = 3;
		std::vector<uint32> buffers;
		uint16 channelCount = 0;
		uint32 frequency = 0;
		int32 format = 0;

		void Init();
		bool Update();
		void Deinit();

		void Seek(float time);

	protected:
		std::ifstream stream;
		uint8* temp = (uint8*)malloc(4096);

		uint32& source;
	};

	virtual void LoadBuffer(string path);
	virtual void LoadStream(string path);

	// @todo format differences?
//	virtual void SaveFile(string path);

	void Play();
	void Pause();
	void Stop();

	float GetVolume();
	void SetVolume(float volume);

	float GetPitch();
	void SetPitch(float pitch);

	string name = "";

protected:

	float GetFloat(int type);

	void SetPosition(vec3 pos);

	uint32 id = 0;

	AudioSource* source = nullptr;

	ptr<Sound::Resource> resource;


private:
};

}
