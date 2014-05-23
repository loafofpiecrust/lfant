
#pragma once

#include <lfant/Component.h>

namespace irrklang {
class ISoundSource;
class ISound;
}

namespace lfant
{

class Rigidbody;

class AudioSource : public Component
{
public:

	void LoadFile(string file);

	void Load(Properties* prop);

	void Init();
	void Update();
	void Deinit();

	void PlayAtPosition(string file);
	void PlayAtPosition(string file, vec3 pos);

	float GetVolume();
	void SetVolume(float vol);

private:

	void OnSetPosition(vec3 pos);

	float volume = 1.0f;
	std::vector<irrklang::ISound*> sounds;

	// Comps
	Rigidbody* rigidbody;
};

}
