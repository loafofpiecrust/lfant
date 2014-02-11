
#pragma once
#include <lfant/stdafx.h>

// internal
#include <lfant/Component.h>
#include <lfant/Rigidbody.h>

// external

namespace lfant {
namespace editor {

class Movement : public Component
{
	DECLARE_TYPE(Component, editor::Movement)
public:

	virtual void Load(Properties* prop);
	virtual void Save(Properties* prop);

	virtual void Init();
	virtual void Update();
	virtual void Deinit();

	void Move(vec3 velocity);

protected:
	float movementSpeed = 5.0f;
	float jumpHeight = 10.0f;
	bool usePhysics = false;

	Rigidbody* rigidbody = nullptr;

private:
};

}
}
