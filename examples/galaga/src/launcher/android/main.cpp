#include <lfant/platform/mobile/android_native_app_glue.h>
#include <lfant/Game.h>

//extern struct android_app* __state;

/**
 * Main entry point.
 */
void android_main(struct android_app* state)
{
    // Android specific : Dummy function that needs to be called to 
    // ensure that the native activity works properly behind the scenes.
    app_dummy();
    
//  __state = state;
    lfant::android::Launch(state);
    
    // Android specific : the process needs to exit to trigger
    // cleanup of global and static resources (such as the game).
    exit(0);
}
