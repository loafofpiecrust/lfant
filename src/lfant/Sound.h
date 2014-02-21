
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
		uint32_t id = 0;
	};

	class Stream : public Resource
	{
	public:
		Stream(uint32_t& source, string path);

		uint32_t bufferCount = 3;
		std::vector<uint32_t> buffers;
		uint16_t channelCount = 0;
		uint32_t frequency = 0;
		int32_t format = 0;

		void Init();
		bool Update();
		void Deinit();

		void Seek(float time);

	protected:
		std::ifstream stream;
		uint8_t* temp = (uint8_t*)malloc(4096);

		uint32_t& source;
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

	uint32_t id = 0;

	AudioSource* source = nullptr;

	ptr<Sound::Resource> resource;


private:
};

}
