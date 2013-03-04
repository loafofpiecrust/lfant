
#include "lfant/gui/System.h"

// External

// Internal
#include <lfant/Engine.h>
#include <lfant/Time.h>

namespace lfant
{
namespace gui
{

System::System()
{
}

System::~System()
{
}

float System::GetElapsedTime()
{
	return game->time->lastFrame;
}

}
}
