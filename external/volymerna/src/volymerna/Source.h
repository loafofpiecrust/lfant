
// internal
#include <volymerna/File.h>

// external
#include <deque>
#include <lfant/ptr.h>
#include <glm/vec3.hpp>

namespace volymerna {

class SoundManager;

class Source
{
	friend class Listener;
	friend class Sound;
public:
	Source();
	virtual ~Source();

	virtual void Update();

//	Sound* GetCurrent();
	File* GetSound(string name);

	void LoadFile(string name, File* input);

//	static std::deque<Source*> sources;
protected:

	glm::vec3 position {0.0f};
	glm::vec3 velocity {0.0f};

	float volume = 1.0f;
	float radius = 1.0f;

	std::deque<lfant::ptr<File>> files;
	File* current = nullptr;

private:
};

}
