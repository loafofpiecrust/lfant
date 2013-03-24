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

// Internal

#include <lfant/Component.h>
#include <lfant/Range.h>
#include <lfant/Material.h>
#include <lfant/Vertex.h>

namespace lfant
{
class Particle;

/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Particles
 *	 @{
 */

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
class ParticleSystem : public Component
{
	friend class Renderer;
public:

	/**
	 *	A burst of particles that occurs at a certain interval.
	 */
	struct Burst
	{
		double time;
		double last;
		uint32_t particles;
	};

	/**
	 *	Supported emitter types.
	 */
	enum class EmitterType
	{
		Point,
		Box,
		Sphere,
		Cone
	};

	enum class DisplayType
	{
		Point,
		Billboard
	};

	ParticleSystem();
	virtual ~ParticleSystem();

	virtual void Init();
	virtual void Update();
	virtual void OnDestroy();

	void Save(Properties* prop);
	void Load(Properties *prop);

	/**
	 *	Emits a specific amount of particles
	 *	@param amount The amount of particles to emit.
	 */
	void Emit(uint32_t amount = 1);
	void Emit(Particle* pt, bool old = false);
	void Recycle(Particle* pt);
	void Clear();
	uint32_t GetCount();
	void GenerateVelocity(Particle* pt);

	// Particle properties
	Range<float> lifetime;
	Range<Range<rgba> > color;
	Range<Range<float> > size;
	Range<Range<float> > speed;
	Range<Range<vec3> > velocity;

	// Emitter properties
	float radius = 1.0f;
	float angle;
	float startDelay;
	float time;
	float duration;
	float timeScale = 1.0f;
	uint32_t rate = 1;
	uint32_t maxParticles = 1500;
	vec3 dimensions = vec3(1.0f);
	vec3 gravity = vec3(0.0f, -9.81f, 0.0f);
	bool pausable = true;
	bool paused = false;
	bool looping = true;

	DisplayType displayType;
	EmitterType emitterType;

	deque< ptr<Particle> > particles;
	vector<Burst> bursts;

	Material material;

protected:
	float toEmit = 0.0f;

private:
	vector<vec3> points;
	vector<Vertex> vertices;
	vector<uint32_t> indices;

	uint32_t vertexBuffer;
	uint32_t indexBuffer;
};

/** @} */
/** @} */
}
