/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2012-08-05 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
#include <lfant/Thread.h>

// External

// Internal

namespace lfant {
namespace thread {

#if WINDOWS
void Sleep(uint32 milli)
{
	::Sleep(milli);
}

#elif UNIX

void Sleep(uint32 milli)
{
	usleep(milli * 1000);
}
#endif

}
}
