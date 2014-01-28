
// internal
#include <lfant/ptr.h>

// external

namespace volymerna {

class Sound
{
public:
	enum class Mode : uint8_t
	{
		Normal,
		Loop,
		Bounce
	};

	Sound(string n, File* f, Source* src) : name(n), file(f), source(src) {}

	void Play();
	void Pause();
	void Stop();

	void PlayCurrentFrame(float dist);

	File* file = nullptr;
	string name = "";
	float time = 0.0f;
	bool playing = false;
	Mode mode = Mode::Normal;
	Source* source = nullptr;
};

class Source
{
	friend class Listener;
	friend class Sound;
public:
	Source();
	virtual ~Source();

	virtual void Update();

	Sound* GetCurrent();
	Sound* GetSound(string name);

	void LoadFile(string name, File* input);

	static deque<Source*> sources;
protected:

	vec3 position {0.0f};
	vec3 velocity {0.0f};

	float volume = 1.0f;
	float radius = 1.0f;

	deque<ptr<Sound>> sounds;
	Sound* current = nullptr;

private:
};

}