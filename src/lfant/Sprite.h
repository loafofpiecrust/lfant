/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-07-28 by Taylor Snead
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
#include <lfant/Mesh.h>
#include <lfant/Material.h>
#include <lfant/Rect.h>

namespace lfant
{
/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Rendering
 *	 @{
 */

/**
 *	Sprite renders a textured material on a plane.
 *		Also, it uses SpriteAnimation instances to animate itself. These animations
 *		change the texture to a different part of the sprite-sheet assigned to the
 *		playing animation.
 *	@todo
 *		Have animations
 */
class Sprite : public Mesh
{
	DECLARE_TYPE(Component, Sprite)
	friend class Renderer;
	friend class SpriteAnimation;
public:

	/**
	 *	An animation that consists of a single texture as a sprite-sheet.
	 *	@details
	 *		A SpriteAnimation has one material of one texture, and for each frame,
	 *		it moves by frameSize, at frameRate times per second. playMode determines
	 *		what the animation will do when it reaches its end. It can either stop,
	 *		loop, or reverse to play forward when it reaches its beginning.
	 */
	class Animation
	{
	public:
		enum class Mode : byte
		{
			Loop = 0,
			Bounce,
			Once,
			Default = Loop
		};

		string name = "Animation";
		Material material;
		uint16_t rows = 5;
		uint16_t columns = 10;
		Mode mode = Mode::Default;
		uint16_t frameRate = 5;
	};

	Sprite();
	virtual ~Sprite();

	// Loop functions
	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Deinit();

	virtual void Load(Properties *props);

	/// The sprite animations to be used.
	deque<Animation> animations;

protected:

	void SetUV(uint32_t idx, vec2 value);

	/// The currently playing animation.
	Animation* currentAnim = nullptr;

//	vec2 currentFrame = vec2(0.0f, 0.0f);
	Rect currentFrame;

	double currentTime = 0.0f;

	bool playingAnim = false;
	bool playingReverseAnim = false;

	Animation::Mode animMode = Animation::Mode::Default;
	Animation::Mode currAnimMode = Animation::Mode::Default;

private:
};

/** @} */
/** @} */
}
