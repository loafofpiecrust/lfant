
#pragma once
#include <lfant/stdafx.h>

// internal
#include <lfant/Component.h>

// external

namespace lfant {
namespace editor {

class Player : public Component
{
	DECLARE_TYPE(Component, editor::Player)
public:

	virtual void Load(Properties* prop);
	virtual void Save(Properties* prop);

	virtual void Init();
	virtual void Update();
	virtual void Deinit();

protected:

private:
};

}
}
