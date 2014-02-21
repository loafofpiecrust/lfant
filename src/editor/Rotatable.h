
#pragma once

// internal
#include <lfant/Component.h>

// external

namespace lfant {
namespace editor {

class Rotatable : public Component
{
public:
	Rotatable();

	virtual void Update();
};

} // namespace editor
} // namespace lfant

