
#pragma once
#include <lfant/stdafx.h>

// internal

// external
#include <boost/filesystem.hpp>

namespace lfant {

class File
{
public:
	File();
	File(string path);

	bool Load(string f);

	float GetSize();

protected:

private:
	boost::filesystem::path path;
};

}