
#include <lfant/AudioSource.h>

#include <lfant/Audio.h>
#include <lfant/Rigidbody.h>
#include <lfant/Transform.h>
#include "Transform.h"

#include <ik_ISoundEngine.h>

namespace lfant {

void AudioSource::LoadFile(string file)
{
	vec3 pos = owner->transform->GetWorldPosition();
	irrklang::ISound* snd = GetGame()->audio->engine->play3D(file.c_str(), {pos.x,pos.y,pos.z}, false);
	sounds.push_back(snd);
	snd->setVolume(volume);
}

void AudioSource::Load(Properties* prop)
{
	lfant::Component::Load(prop);
	std::vector<string> files;
	prop->Get("files", files);
	for(auto& f : files)
	{
		LoadFile(f);
	}
}
	
void AudioSource::Init()
{
	ConnectEvent(SENDER(owner->transform, SetPosition), RECEIVER(this, OnSetPosition));
	ConnectComponent<Rigidbody>(rigidbody);
}

void AudioSource::Update()
{
}

void AudioSource::Deinit()
{
}

void AudioSource::OnSetPosition(vec3 pos)
{
	vec3 wpos = owner->transform->GetWorldPosition();
	for(irrklang::ISound*& snd : sounds)
	{
		snd->setPosition({wpos.x, wpos.y, wpos.z});
		if(rigidbody)
		{
			snd->setVelocity(vec3_cast<irrklang::vec3df>(rigidbody->GetVelocity()));
		}
	}
}

void AudioSource::PlayAtPosition(string file, vec3 pos)
{
	GetGame()->audio->engine->play3D(file.c_str(), {pos.x,pos.y,pos.z}, false);
}

void AudioSource::PlayAtPosition(string file)
{
	PlayAtPosition(file, owner->transform->GetWorldPosition());
}

float AudioSource::GetVolume()
{
	return volume;
}

void AudioSource::SetVolume(float vol)
{
	volume = vol;
	for(irrklang::ISound*& snd : sounds)
	{
		snd->setVolume(volume);
	}
}

	
}