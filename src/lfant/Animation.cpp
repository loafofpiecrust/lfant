
#include <lfant/Animation.h>

namespace lfant {

void Animation::Clip::Save(Properties* prop) const
{
	prop->type = "Clip";
	prop->id = name;
	prop->Set("start", start);
	prop->Set("end", end);
	prop->Set("frameRate", frameRate);
}

void Animation::Clip::Load(Properties* prop)
{
	name = prop->id;
	prop->Get("start", start);
	prop->Get("end", end);
	prop->Get("frameRate", frameRate);
}

void Animation::Save(Properties* prop) const
{
	Component::Save(prop);

	for(auto& clip : clips)
	{
		clip->Save(prop->AddChild("Clip"));
	}
}

void Animation::Load(Properties* prop)
{
	for(Properties*& child : prop->GetChildren("Clip"))
	{
		Clip* clip = new Clip();
		clip->Load(child);
		clips.push_back(clip);
	}
}

void Animation::AddClip(Clip* clip)
{
	clips.push_back(clip);
}

Animation::Clip* Animation::GetClip(string name)
{
	for(auto& clip : clips)
	{
		if(clip->name == name)
		{
			return clip;
		}
	}
	return nullptr;
}

}
