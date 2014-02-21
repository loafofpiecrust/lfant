
#include <volymerna/Source.h>

// internal
#include <volymerna/System.h>
#include <volymerna/Stream.h>
#include "volymerna/Sound.h"

// external

namespace volymerna {


/*******
 *	Source
 ******/
Source::Source()
{

}

Source::~Source()
{
}

void Source::LoadFile(string name, File* input)
{
	files.push_back(input);
	input->source = this;

	std::shared_ptr<File> file {input};
	for(auto& l : System::Get()->listeners)
	{
		file->sounds.push_back(new Sound(file));
	}


}

void Source::Update()
{
}

/*
Sound* Source::GetCurrent()
{
	return current;
}
*/
File* Source::GetSound(string name)
{
	for(auto& snd : files)
	{
		if(snd->name == name)
		{
			return snd;
		}
	}
}

}
