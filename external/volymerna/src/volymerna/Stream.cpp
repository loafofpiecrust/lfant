
#include <volymerna/Stream.h>

// internal
#include <volymerna/System.h>
#include <volymerna/Listener.h>

// external
#include <AL/al.h>
#include <AL/alc.h>

namespace volymerna {

Stream::Stream(std::string path, Source* src) :
	File(src)
{
	file.open(path);

	std::shared_ptr<File> f {(File*)this};
	for(uint i = 0; i < System::Get()->listeners.size(); ++i)
	{
		f->sounds.push_back(new Sound(f));
	}
}

Stream::~Stream()
{
}

void Stream::PlayChunk()
{
	if(file.eof())
	{
		return;
	}

//	uint32_t buffer = 0;
//	int32_t val = 0;
//	int ret = 0;

	/// @todo Implement file streaming
	/*
	alGetSourcei(source, AL_BUFFERS_PROCESSED, &val);
	if(val <= 0)
		return true;

	while(--val && !file.eof())
	{
		file.read((char*)temp, 4096);

		alSourceUnqueueBuffers(source, 1, &buffer);
		alBufferData(buffer, format, temp, ret, frequency);
		alSourceQueueBuffers(source, 1, &buffer);
		// check errors
	}

	// Remove to allow pausing?
	alGetSourcei(source, AL_SOURCE_STATE, &val);
	if(val != AL_PLAYING)
		alSourcePlay(source);
	*/
}

void Stream::Seek(float time)
{

}

}
