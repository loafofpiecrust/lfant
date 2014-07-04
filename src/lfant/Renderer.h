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

// External
#include <deque>

// Internal
#include <lfant/Range.h>
#include <lfant/Shader.h>
#include <lfant/Vertex.h>

#include <lfant/Subsystem.h>

//struct GLFWwindow;

namespace sf {
class Window;
}

namespace lfant {

class Geometry;
class Sprite;
class Light;
class ParticleSystem;
class Material;
class FrameBuffer;
class Entity;
class Light;

/**	@addtogroup Game
 *	@{
 */
/**	@addtogroup Rendering
 *	@{
 */

/**	This class handles the base Rendering, it should be called by the GameStart class.
 *		This class currently handles OpenGL via GLFW and GLEW, but
 *		it can be configured to use DirectX, if one so desires.
 *		The OpenGL version variable is held here, and a settings
 *		class should change them for use, then reload the rendering
 *		system.
 */
class Renderer : public Subsystem
{
	friend class Game;
	friend class Light;
public:
	Renderer(Game* game);
	~Renderer();

	virtual void Serialize(Properties* prop);


	virtual void Init();
	virtual void Update();
	virtual void Deinit();

	void SetRendering(bool render);

	void Resize(ivec2 size);

	// Shaders
	Shader* GetShader(string name);
	void AddShader(Shader* shader);

	void AddLight(Light* light);
	void RemoveLight(Light* light);

	void IndexArray(std::vector<vec3>& arr, std::vector<uint32>& idx);

	/// Whether or not the frame-rate should try to sync with the monitor refresh rate (60 hz/fps)
	bool vsync = true;

	/// The amount of texture interpolation to use
	byte texInterp;

	/// Whether or not to render.
	bool render = true;

	float gamma = 2.2f;

	int motionBlur = 5;

	float ambientLight = 0.1f;

	ptr<FrameBuffer> frameBuffer;
	ptr<FrameBuffer> lightBuffer;

protected:

	std::deque<ptr<Shader>> shaders;
	std::deque<Light*> lights;
};

/// @}
/// @}
}
