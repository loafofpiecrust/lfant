
#pragma once
#include <lfant/stdafx.h>

// internal
#include <lfant/Component.h>

// external

namespace lfant {
namespace editor {

class MouseLook : public Component
{
public:
	virtual void Load(Properties* prop);
	virtual void Save(Properties* prop);

	virtual void Init();

	void OnSetMousePos(ivec2 pos);
	void Look(vec3 axis, float value);

protected:
	float lookSpeed = 0.01f;
	ivec2 lastMouse = ivec2(0);

private:
};

}
}
