
#pragma once
#include <lfant/stdafx.h>

// internal
#include <lfant/Component.h>

// external

namespace lfant {
namespace editor {

class Player : public Component
{
public:

	virtual void Serialize(Properties* prop);

	virtual void Init();
	virtual void Update();
	virtual void Deinit();
protected:

private:
};

}
}
