/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
#pragma once
#include <lfant/stdafx.h>

// Internal
#include <lfant/Light.h>
#include <lfant/Mesh.h>

// External

namespace lfant {

class PointLight : public Light
{
public:
	PointLight();
	~PointLight();

	virtual void Save(Properties* prop) const;
	virtual void Load(Properties* prop);

	virtual void Init();
	void PostUpdate();

	void BeginRender();
	virtual void Render();
	void EndRender();

	virtual void LoadFile(string path);

protected:
	float ambientIntensity = 1.0f;
	float diffuseIntensity = 1.0f;
	float attenConst = 1.0f;
	float attenLinear = 1.0f;
	float attenExp = 1.0f;
	float specularPower = 1.0f;
	float specularIntensity = 1.0f;
	float radius = 1.0f;

	Buffer<vec3> posBuffer;
	Buffer<vec2> uvBuffer;
	Buffer<uint32_t> indexBuffer;

private:
};

}
