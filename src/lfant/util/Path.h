/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2013-04-27 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
#pragma once
#include <lfant/stdafx.h>

// Internal

// External
#include <boost/filesystem.hpp>

using namespace boost::filesystem;

namespace lfant {

path operator+(path const& p, string s);
//path operator+(path& p, string s);

//operator string(const path& p);

//operator const char*(const path& p);

}
