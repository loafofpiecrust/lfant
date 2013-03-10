
#pragma once
#include <lfant/stdafx.h>

// External

// Internal
#include <lfant/Component.h>

namespace lfant
{

/**
 *
 */
class Renderable : public Component
{
public:
	virtual ~Renderable();

protected:
	Renderable();

	virtual void Init();
	virtual void PostUpdate();
	virtual void OnDestroy();

	virtual void BeginRender();
	virtual void Render() = 0;
	virtual void EndRender();

	bool loaded = false;
};

}
