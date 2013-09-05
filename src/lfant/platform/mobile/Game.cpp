
// Internal
#include <lfant/Game.h>
#include <lfant/Input.h>

// External
#include <jni.h>
#include "android_native_app_glue.h"
#include <android/log.h>
#include <android/sensor.h>
#include <android/input.h>
#include <GL/glew.h>
#include <EGL/egl.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

void app_dummy()
{
}

namespace lfant {

struct saved_state
{
	// Save data?
};

class AppEngine
{
public:

	void Init();
	void Update();
	void Tear();

	android_app* app;

	ASensorManager* sensorManager;
	const ASensor* accelerometerSensor;
	ASensorEventQueue* sensorEventQueue;

	int animating;
	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	int32_t width;
	int32_t height;
	saved_state state;
};

void AppEngine::Init()
{
	// initialize OpenGL ES and EGL

	/*
	 * Here specify the attributes of the desired configuration.
	 * Below, we select an EGLConfig with at least 8 bits per color
	 * component compatible with on-screen windows
	 */
	const EGLint attribs[] = {
		EGL_SURFACE_TYPE,
		EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_NONE
    };
	EGLint dummy, format;
	EGLint numConfigs;
	EGLConfig config;

	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	eglInitialize(display, 0, 0);

	/* Here, the application chooses the configuration it desires. In this
	 * sample, we have a very simplified selection process, where we pick
	 * the first EGLConfig that matches our criteria */
	eglChooseConfig(display, attribs, &config, 1, &numConfigs);

	/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
	 * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
	 * As soon as we picked a EGLConfig, we can safely reconfigure the
	 * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(this->app->window, 0, 0, format);

	surface = eglCreateWindowSurface(display, config, this->app->window, NULL);
	context = eglCreateContext(display, config, NULL, NULL);

	if(eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
	{
		LOGW("Unable to eglMakeCurrent");
		return;
	}

	eglQuerySurface(display, surface, EGL_WIDTH, &width);
	eglQuerySurface(display, surface, EGL_HEIGHT, &height);

//	this->state.angle = 0;

	// Initialize GL state.
//	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glEnable(GL_CULL_FACE);
//	glShadeModel(GL_SMOOTH);
	glDisable(GL_DEPTH_TEST);

//	return;
}

void AppEngine::Update()
{
	if(!display)
	{
		return;
	}

	// Rendering
//	glClearColor(((float)state.x)/width, state.angle, ((float)state.y)/height, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	eglSwapBuffers(display, surface);
}

void AppEngine::Tear()
{
	if (display != EGL_NO_DISPLAY)
	{
		eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (context != EGL_NO_CONTEXT)
		{
			eglDestroyContext(display, context);
		}
		if (surface != EGL_NO_SURFACE)
		{
			eglDestroySurface(display, surface);
		}
		eglTerminate(display);
	}
	animating = 0;
	display = EGL_NO_DISPLAY;
	context = EGL_NO_CONTEXT;
	surface = EGL_NO_SURFACE;
}

static int32_t OnInput(android_app* app, AInputEvent* event)
{
	AppEngine* engine = (AppEngine*)app->userData;
	switch(AInputEvent_getType(event))
	{
	case AINPUT_EVENT_TYPE_MOTION:
	{
		engine->animating = 1;
		uint32_t count = AMotionEvent_getPointerCount(event);
		int32_t action = AMotionEvent_getAction(event);
		for(uint32_t i = 0; i < count; ++i)
		{
			vec2 pos = vec2(AMotionEvent_getX(event, i), AMotionEvent_getY(event, i));
			game->input->OnTouch(i, action, {pos});
		}
		return 1;
	}
	}
	return 0;
}

static void OnMain(struct android_app* app, int32_t cmd)
{
	AppEngine* engine = (AppEngine*)app->userData;
	switch (cmd)
	{
	case APP_CMD_SAVE_STATE:
		// The system has asked us to save our current state.  Do so.
	//	engine->app->savedState = malloc(sizeof(struct saved_state));
	//	*((struct saved_state*)engine->app->savedState) = engine->state;
	//	engine->app->savedStateSize = sizeof(struct saved_state);
		break;
	case APP_CMD_INIT_WINDOW:
		// The window is being shown, get it ready.
		if(engine->app->window)
		{
			engine->Init();
			engine->Update();
		}
		break;
	case APP_CMD_TERM_WINDOW:
		// The window is being hidden or closed, clean it up.
		engine->Tear();
		break;
	case APP_CMD_GAINED_FOCUS:
		// When our app gains focus, we start monitoring the accelerometer.
		if (engine->accelerometerSensor != NULL) {
			ASensorEventQueue_enableSensor(engine->sensorEventQueue, engine->accelerometerSensor);
			// We'd like to get 60 events per second (in us).
			ASensorEventQueue_setEventRate(engine->sensorEventQueue, engine->accelerometerSensor, (1000L/60)*1000);
		}
		break;
	case APP_CMD_LOST_FOCUS:
		// When our app loses focus, we stop monitoring the accelerometer.
		// This is to avoid consuming battery while not being used.
		if (engine->accelerometerSensor != NULL)
		{
			ASensorEventQueue_disableSensor(engine->sensorEventQueue, engine->accelerometerSensor);
		}
		// Also stop animating.
		engine->animating = 0;
		engine->Update();
		break;
	}
}

void android_main(android_app* state)
{
	AppEngine engine;

	// ?
	app_dummy();

	memset(&engine, 0, sizeof(AppEngine));
	state->userData = &engine;
	state->onAppCmd = OnMain;
	state->onInputEvent = OnInput;
	engine.app = state;

	// Prepare to monitor accelerometer
	engine.sensorManager = ASensorManager_getInstance();
	engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager, ASENSOR_TYPE_ACCELEROMETER);
	engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager, state->looper, LOOPER_ID_USER, NULL, NULL);

	if(state->savedState)
	{
		// We are starting with a previous saved state; restore from it.
	//	engine.state = *(saved_state*)state->savedState;
	}

	// loop waiting for stuff to do.
	while (1)
	{
		// Read all pending events.
 		int ident;
		int events;
		android_poll_source* source;

		// If not animating, we will block forever waiting for events.
		// If animating, we loop until all events are read, then continue
		// to draw the next frame of animation.
		while ((ident=ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events, (void**)&source)) >= 0)
		{
			// Process this event.
			if (source)
			{
				source->process(state, source);
			}

			// If a sensor has data, process it now.
			if (ident == LOOPER_ID_USER) 
			{
				if (engine.accelerometerSensor != NULL)
				{
					ASensorEvent event;
					while (ASensorEventQueue_getEvents(engine.sensorEventQueue, &event, 1) > 0)
					{
						LOGI("accelerometer: x=%f y=%f z=%f", event.acceleration.x, event.acceleration.y, event.acceleration.z);
					}
				}
			}

			// Check if we are exiting.
			if (state->destroyRequested != 0)
			{
				engine.Tear();
				return;
			}
		}

		if (engine.animating)
		{
			// Done with events; draw next animation frame.
		//	engine.state.angle += .01f;
		//	if (engine.state.angle > 1) 
		//	{
		//		engine.state.angle = 0;
		//	}

			// Drawing is throttled to the screen update rate, so there
			// is no need to do timing here.
			engine.Update();
		}
	}
}

}