#pragma once

#include "lfant/Renderable.h"
#include "lfant/Material.h"

namespace lfant
{

class Geometry;
class Material;

class Mesh : public Renderable
{
public:
	Mesh();

	virtual void Serialize(Properties *prop);

	virtual void Init();
	virtual void Render();
	virtual void Deinit();

protected:
	std::shared_ptr<Geometry> mesh;
	Material material;

	bool usingCamera = true;
};

} // namespace lfant

