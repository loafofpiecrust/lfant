
#include <lfant/Renderable.h>

// Internal
#include <lfant/Console.h>

// External

namespace lfant
{

Renderable::Renderable()
{
}

Renderable::~Renderable()
{
}

void Renderable::Init()
{
	Component::Init();
//	BeginRender();
}

void Renderable::PostUpdate()
{
	Component::PostUpdate();
	if(loaded)
	{
		Render();
	}
}

void Renderable::OnDestroy()
{
	Component::OnDestroy();
	if(loaded)
	{
		EndRender();
	}
}

void Renderable::BeginRender()
{
	loaded = true;
}

void Renderable::EndRender()
{
	loaded = false;
}

}
