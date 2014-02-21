
#pragma once
#include <lfant/stdafx.h>

// Internal
#include <lfant/Material.h>

// External
#include <LinearMath/btIDebugDraw.h>

namespace lfant {
namespace physics {

class DebugRenderer : public btIDebugDraw
{
public:
	DebugRenderer();
	~DebugRenderer();

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

	void Render();

protected:
	std::vector<btVector3> points;

	ptr<Material> material;
	uint32_t pointBuffer = 0;
	uint32_t vertexArray = 0;

private:
};

}
}
