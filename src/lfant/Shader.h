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
	virtual void Save(Properties* prop) const;
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
	void SetUniform(string name, int val);
	void SetUniform(string name, unsigned int val);
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

	static std::deque<Shader*> shaders;
	static Shader* current;
};

/// @}
/// @}
}
