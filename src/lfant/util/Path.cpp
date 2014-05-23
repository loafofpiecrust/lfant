/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2013-04-27 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/

#include <lfant/util/Path.h>

// Internal

// External

namespace lfant {

boost::filesystem::path operator+(boost::filesystem::path const& p, string s)
{
	return p.string()+s;
}

}
