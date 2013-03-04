
#include <lfant/gui/FileSystem.h>

// External

// Internal
#include <lfant/Engine.h>
#include <lfant/Time.h>
#include <lfant/FileSystem.h>

namespace lfant
{
namespace gui
{

FileSystem::FileSystem()
{
}

FileSystem::~FileSystem()
{
}

// Opens a file.
Rocket::Core::FileHandle FileSystem::Open(const Rocket::Core::String& path)
{
	// Attempt to open the file relative to the application's root.
	FILE* fp = fopen(game->fileSystem->GetGamePath(path.CString()).c_str(), "rb");
	if (fp != NULL)
		return (Rocket::Core::FileHandle) fp;

	// Attempt to open the file relative to the current working directory.
	fp = fopen(path.CString(), "rb");
	return (Rocket::Core::FileHandle) fp;
}

// Closes a previously opened file.
void FileSystem::Close(Rocket::Core::FileHandle file)
{
	fclose((FILE*) file);
}

// Reads data from a previously opened file.
size_t FileSystem::Read(void* buffer, size_t size, Rocket::Core::FileHandle file)
{
	return fread(buffer, 1, size, (FILE*) file);
}

// Seeks to a point in a previously opened file.
bool FileSystem::Seek(Rocket::Core::FileHandle file, long offset, int origin)
{
	return fseek((FILE*) file, offset, origin) == 0;
}

// Returns the current position of the file pointer.
size_t FileSystem::Tell(Rocket::Core::FileHandle file)
{
	return ftell((FILE*) file);
}

}
}
