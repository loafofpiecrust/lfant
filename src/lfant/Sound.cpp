
#include <lfant/Sound.h>

// internal
#include <lfant/File.h>
#include <lfant/AudioSource.h>
#include <lfant/FileSystem.h>

// external
#include <AL/al.h>
#include <AL/alc.h>

namespace lfant {

Sound::Sound(AudioSource* src, string name) :
	source(src),
	name(name)
{
	alGenSources(1, &id);
	alSourcei(id, AL_BUFFER, 0);
}

Sound::~Sound()
{
	alSourcei(id, AL_BUFFER, 0);
	alDeleteSources(1, &id);
}

void Sound::LoadBuffer(string path)
{
	path = game->fileSystem->GetGamePath(path).string();
	Sound::Buffer* res = new Sound::Buffer();

	alGenBuffers(1, &res->id);
	alSourcei(id, AL_BUFFER, res->id);

	resource = res;
}

void Sound::LoadStream(string path)
{
	path = game->fileSystem->GetGamePath(path).string();
	Sound::Stream* res = new Sound::Stream(id, path);
	res->Init();

	resource = res;
}

void Sound::Play()
{
	alSourcePlay(id);
}

void Sound::Pause()
{
	alSourcePause(id);
}

void Sound::Stop()
{
	alSourceStop(id);
}

float Sound::GetFloat(int type)
{
	float val = 1.0f;
	alGetSourcef(id, type, &val);
	return val;
}

float Sound::GetVolume()
{
	return GetFloat(AL_GAIN);
}

void Sound::SetVolume(float volume)
{
	alSourcef(id, AL_GAIN, volume);
}

float Sound::GetPitch()
{
	return GetFloat(AL_PITCH);
}

void Sound::SetPosition(vec3 pos)
{
	alSource3f(id, AL_POSITION, pos.x, pos.y, pos.z);
}


////////
//	Sound::Stream
///////

Sound::Stream::Stream(uint32_t& source, string path) :
	source(source),
	stream(path, ios::in|ios::binary)
{
}

void Sound::Stream::Init()
{
	buffers.resize(bufferCount);
	alGenBuffers(buffers.size(), &buffers[0]);

	uint8_t data[8];
	stream.seekg(12, ios::cur);

	stream.read((char*)data, 8);
	if(data[0] != 'f' || data[1] != 'm' || data[2] != 't')
	{
		// Not wav format
		stream.close();
		return;
	}

	// reading time
	stream.read((char*)data, 2);
	if(data[0] != 1 || data[1] != 0)
	{
		// not PCM?
	}

	// channel count
	stream.read((char*)&channelCount, sizeof(channelCount));

	// sample frequency
	stream.read((char*)&frequency, sizeof(frequency));

	// skip block size and bytes/sec
	stream.seekg(6, ios::cur);

	// bit depth?
	int32_t bits = 0;
	stream.read((char*)&bits, sizeof(bits));

	if(bits == 8)
	{
		if(channelCount == 1)
		{
			format = AL_FORMAT_MONO8;
		}
		else
		{
			format = AL_FORMAT_STEREO8;
		}
	}
	else if(bits == 16)
	{
		if(channelCount == 1)
		{
			format = AL_FORMAT_MONO16;
		}
		else
		{
			format = AL_FORMAT_STEREO16;
		}
	}
	if(format == 0)
	{
		// unsupported format?
	}

	int ret = 0;
//	temp = malloc(4096);

	for(uint i = 0; i < buffers.size(); ++i)
	{
		stream.read((char*)temp, 4096);
		alBufferData(buffers[i], format, temp, ret, frequency);
	}
	// check errors?

	// @todo Get source id here somehow
	alSourceQueueBuffers(source, buffers.size(), &buffers[0]);
	alSourcePlay(source);
	// check errors
}

bool Sound::Stream::Update()
{
	if(stream.eof())
	{
		return false;
	}

	uint32_t buffer = 0;
	int32_t val = 0;
	int ret = 0;

	alGetSourcei(source, AL_BUFFERS_PROCESSED, &val);
	if(val <= 0)
		return true;

	while(--val && !stream.eof())
	{
		stream.read((char*)temp, 4096);

		alSourceUnqueueBuffers(source, 1, &buffer);
		alBufferData(buffer, format, temp, ret, frequency);
		alSourceQueueBuffers(source, 1, &buffer);
		// check errors
	}

	// Remove to allow pausing?
	alGetSourcei(source, AL_SOURCE_STATE, &val);
	if(val != AL_PLAYING)
		alSourcePlay(source);
}

void Sound::Stream::Seek(float time)
{
	time = time*frequency;
	stream.seekg(floor(time));
}

}