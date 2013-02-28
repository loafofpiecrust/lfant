
#include "lfant/gui/System.hpp"

// External

// Internal
#include <lfant/Engine.hpp>
#include <lfant/Time.hpp>

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
