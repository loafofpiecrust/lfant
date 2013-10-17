/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-08-02 by Taylor Snead
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

// External
#include <boost/compute/container/vector.hpp>

// Internal
#include <lfant/Component.h>
#include <lfant/Range.h>
#include <lfant/Material.h>
#include <lfant/Vertex.h>
#include <lfant/Mesh.h>
#include <lfant/OpenCL.h>

namespace lfant
{
//class Particle;

/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Particles
 *	 @{
 */

class Particle
{
public:
	vec4 pos;
	vec4 color;
	vec4 vel;
	vec4 velChange;
	vec4 colorChange;
	float sizeChange;
	float life;
	float size;
} __attribute__ ((aligned (16)));

/**	This class handles a ParticleSystem Component
 *		Gives functionality for emitting particles from this transform. The
 *		emitted particles die after a certain lifetime, and while alive, follow
 *		the given parameters. The particles can be rendered either as quads
 *		(really two triangles, of course) or as point-sprites. Point-sprites
 *		are more efficient, but it's possible that some older hardware doesn't
 *		support it.
 *	@todo
 *		Rendering code for ParticleSystem.
 *		Having the ParticleSystem Component send a call to render itself.
 *		Integrate with Bullet Physics for collision detection, bounciness, friction, and maybe buoyancy.
 */
class ParticleSystem : public Renderable
{
	DECLARE_COMP(ParticleSystem)
	friend class Renderer;
	friend class Particle;
public:

	/**
	 *	A burst of particles that occurs at a certain interval.
	 */
	struct Burst
	{
		float time = 5.0f;
		float current;
		uint32_t particles = 10;
	};

	/**
	 *	Supported emitter types.
	 */
	enum class EmitterType : byte
	{
		Point = 0,
		Box,
		Sphere,
		Cone
	};

	enum class DisplayType : byte
	{
		Point,
		Billboard
	};

	struct ParticleVertex
	{
		vec3 position;
		rgba color;
		float size;

		ParticleVertex(vec3 pos, vec4 col, float size) :
			position(pos), color(col), size(size)
		{
		}
	};

	ParticleSystem();
	virtual ~ParticleSystem();

	virtual void Init();
	virtual void Update();
	virtual void PostUpdate();
	virtual void Deinit();

	void BeginRender();
	void Render();
	void EndRender();

	void Save(Properties* prop);
	void Load(Properties *prop);

	/**
	 *	Emits a specific amount of particles
	 *	@param amount The amount of particles to emit.
	 */
	void Emit(uint32_t amount = 1);
	void Recycle(Particle* pt);
	void Clear();
	uint32_t GetCount();

	// Particle properties
	Range<float> lifetime = {1.0f, 5.0f};
	Range<Range<vec4>> color = {{vec4(0), vec4(0.5f)}, {vec4(0.51f), vec4(1)}};
	Range<Range<float>> size = {{0.1f, 1.0f}, {0.5f, 2.0f}};
//	Range<Range<float>> speed;
	Range<Range<vec3>> velocity = {{vec3(0), vec3(5)}, {vec3(2), vec3(10)}};

	// Emitter properties
	float radius = 1.0f;
	float angle = 30.0f;
	float startDelay;
	float time;
	float duration;
//	float timeScale = 1.0f;
	float rate = 1.5f;
	uint32_t maxParticles = 1500;
	vec3 dimensions = vec3(1.0f);
	vec4 gravity = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	bool pausable = true;
	bool paused = false;
	bool looping = true;
	bool inheritTransform = true;

	DisplayType displayType;
	EmitterType emitterType = EmitterType::Cone;

//	deque<Particle> particles;
	uint32_t particleCount = 0;
//	Buffer<Particle> particles;
	deque<Burst> bursts;
	deque<Particle*> recycle;

	ptr<Material> material;

protected:

	void UpdatePosition(Particle* pt);
	void OnSetPosition();

	float toEmit = 0.0f;

private:
//	Buffer<vec4> colorBuffer;
//	Buffer<float> sizeBuffer;
//	Buffer<ParticleVertex> particleBuffer;
	uint32_t colorBuffer;
	uint32_t posBuffer;
	uint32_t sizeBuffer;
	uint32_t vertexArray;

	vector<uint32_t> buffers;
	vector<boost::compute::buffer> clbuffers;

	ptr<OpenCL::Kernel> updateKernel;
	ptr<OpenCL::Kernel> emitKernel;

	bool rewriteBuffer = false;
};

/** @} */
/** @} */
}
