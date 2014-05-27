
#include "System.h"

#include "lfant/Game.h"
#include "lfant/Time.h"

namespace lfant {
namespace gui {

float rocket::System::GetElapsedTime()
{
	return (float)game->time->GetTime();
}

}
}