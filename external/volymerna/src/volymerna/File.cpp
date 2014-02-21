
#include "volymerna/File.h"

// internal
#include "volymerna/System.h"
#include "volymerna/Sound.h"

// external

namespace volymerna {

void File::Update()
{
	currTime += System::Get()->GetDeltaTime();
	for(auto& snd : sounds)
	{
		snd->Update();
		
	}
}



}
