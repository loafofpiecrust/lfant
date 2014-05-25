
#include <lfant/Animation.h>

namespace lfant {

void Animation::Clip::Serialize(Properties* prop)
{
//	name = prop->id;
	prop->Value("name", &name);
	prop->Value("start", &start);
	prop->Value("end", &end);
	prop->Value("frameRate", &frameRate);
}

void Animation::Serialize(Properties* prop)
{
	prop->ValueArray<Clip*>("Clip", clips, [](Clip*& clip, Properties* prop)
	{
		clip->Serialize(prop);
	});
/*	for(auto& child : prop->children)
	{
		if(child->type != "Clip") continue;

		Clip* clip = new Clip();
		clip->Load(child);
		clips.push_back(clip);
	}*/
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
