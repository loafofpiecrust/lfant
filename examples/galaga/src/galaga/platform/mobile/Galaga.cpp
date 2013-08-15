
// Internal
#include <galaga/Galaga.h>

// External
#include <jni.h>

namespace lfant {
namespace galaga {

void Java_com_lazyfox_lfant_MainApp_launch(JNIEnv* env)
{
	game = new Galaga;
	game->Init();
	while(!game->IsExited())
	{
		game->Update();
	}
	game->Destroy();
	delete game;
}

}
}