
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
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);
	void setDebugMode(int debugMode);
	int getDebugMode() const;

	void Render(Game* game);

protected:
	std::vector<btVector3> points;
	std::vector<btVector3> colors;

	ptr<Material> material;
	uint32 pointBuffer = 0;
	uint32 colorBuffer = 0;

	uint32 vertexArray = 0;

	int mode;

private:
};

}
}
