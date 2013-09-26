/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-10-29 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License at
*
*		http://www.apache.org/licenses/LICENSE-2.0
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
#include <lfant/Object.h>
#include <lfant/util/qumap.h>

// External
#include <forward_list>

namespace lfant {

class Texture;

/** @addtogroup Game
 *	@{
 */
/** @addtogroup Rendering
 *	@{
 */

/**
 *
 */
class Shader : public Object
{
public:

	struct Uniform
	{
		string name = "";
		uint32 id = 0;
	};

	Shader()
	{
	}

	virtual void Destroy();

	virtual void Load(Properties *prop);
	virtual void Save(Properties* prop);
//	void LoadFile(string file = "");
	virtual void LoadFile(string file);
	void LoadFile(string vert, string frag, string geom = "", string comp = "");
	void Compile();

	void Bind();
	void Unbind();

	static Shader* GetCurrent();

	uint32 GetId();

	uint32 GetUniform(string name);
	void AddUniform(string name);
	
	void SetUniform(string name, float val);
	void SetUniform(string name, const vec2& val);
	void SetUniform(string name, const vec3& val);
	void SetUniform(string name, const vec4& val);
	void SetUniform(string name, const mat4& val);
	void SetUniform(string name, Texture* val);

protected:

	static uint32 Compile(uint32 type, const string& path);
	void CheckErrors();

	string vertex = "shaders/simple/Diffuse.vert";
	string fragment = "shaders/simple/Diffuse.frag";
	string geometry = "";
	string compute = "";
	uint32 id = -1;
	qumap<string, uint32> uniforms;

	static deque<Shader*> shaders;
	static Shader* current;
};

/// @}
/// @}
}
