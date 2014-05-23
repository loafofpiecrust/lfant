
#include <lfant/File.h>

// internal
#include <lfant/Game.h>
#include <lfant/FileSystem.h>

// external

namespace lfant {

File::File()
{
}

File::File(string path)
{
}

bool File::Load(string f)
{
	path = {f};
	return boost::filesystem::is_regular_file(path);
}

float File::GetSize()
{
	return boost::filesystem::file_size(path);
}

}
