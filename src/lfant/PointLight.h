/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2013-06-22 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
*	Unless required by applicable law or agreed to in writing, software
*	distributed under the License is distributed on an "AS IS" BASIS,
*	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*	See the License for the specific language governing permissions and
*	limitations under the License.
*
******************************************************************************/
#pragma once
#include <lfant/stdafx.h>

// Internal
#include <lfant/Light.h>
#include <lfant/Mesh.h>

// External

namespace lfant {

class PointLight : public Light
{
	DECLARE_COMP(PointLight)
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