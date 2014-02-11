
#pragma once

// internal
#include <lfant/Component.h>

// external

namespace lfant {
namespace editor {

class Rotatable : public Component
{
	DECLARE_TYPE(Component, editor::Rotatable)
public:
	Rotatable();

	virtual void Update();
};

} // namespace editor
} // namespace lfant

