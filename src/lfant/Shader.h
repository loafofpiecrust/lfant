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
#include <unordered_map>
#include <forward_list>
#include <memory>

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
class Shader //: public Object
{
public:
	Shader();
	virtual ~Shader();

	static std::shared_ptr<Shader> Load(Properties* prop);
	void Serialize(Properties* prop);

//	void LoadFile(string file = "");
	static std::shared_ptr<Shader> LoadFile(string vert, string frag, string geom = "", string comp = "");
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
	static void SetCurrent(Shader* sh);

	string vertex = "shaders/simple/Diffuse.vert";
	string fragment = "shaders/simple/Diffuse.frag";
	string geometry;
	string compute;
	uint32 id = -1;
	std::map<string, uint32> uniforms;

	static std::deque<std::weak_ptr<Shader>> shaderCache;
	static Shader* current;
};

/// @}
/// @}
}
