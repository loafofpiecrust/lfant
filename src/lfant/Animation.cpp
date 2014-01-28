
#include <lfant/Animation.h>

namespace lfant {

Animation::Animation()
{
}

Animation::~Animation()
{
}

void Animation::Save(Properties* prop) const
{
	Component::Save(prop);

	for(Clip& clip : clips)
	{
		Properties* child = prop->AddChild("Clip", clip.name);
		child->Set("start", start);
		child->Set("end", end);
	}
}

void Animation::Load(Properties* prop)
{
	for(Properties*& child : prop->GetChildren("Clip"))
	{
		Clip* clip = new Clip();
		clip->name = child->id;
		child->Get("start", clip->start);
		child->Get("end", clip->end);

		clips.push_back(*clip);
	}
}

void Animation::AddClip(string name, uint16_t start, uint16_t end, uint16_t fps)
{
	clips.push_back(Clip(name, start, end, fps));
}

Animation::Clip* Animation::GetClip(string name)
{
	for(auto& clip : clips)
	{
		if(clip.name == name)
		{
			return clip;
		}
	}
	return nullptr;
}

}
