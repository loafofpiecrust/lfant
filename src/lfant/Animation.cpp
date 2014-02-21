
#include <lfant/Animation.h>

namespace lfant {

void Animation::Clip::Save(Properties* prop) const
{
//	prop->type = "Clip";
	prop->Set("name", name);
	prop->Set("start", start);
	prop->Set("end", end);
	prop->Set("frameRate", frameRate);
}

void Animation::Clip::Load(Properties* prop)
{
//	name = prop->id;
	prop->Get("name", name);
	prop->Get("start", start);
	prop->Get("end", end);
	prop->Get("frameRate", frameRate);
}

void Animation::Save(Properties* prop) const
{
	Component::Save(prop);

//	Properties* arr = prop->AddChild("clips");
	for(auto& clip : clips)
	{
		clip->Save(new Properties(prop, "Clip"));
	}
}

void Animation::Load(Properties* prop)
{
	for(auto& child : prop->children)
	{
		if(child->type != "Clip") continue;

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
