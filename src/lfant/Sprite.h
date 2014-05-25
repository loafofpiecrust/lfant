/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
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

	//	string name = "Animation";
		uint32 start = 0;
		uint32 end = 1;
		Mode mode = Mode::Default;
		uint16 frameRate = 5;
	};

	Sprite();
	virtual ~Sprite();

	// Loop functions
	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Deinit();

	virtual void Serialize(Properties *props);

	/// The sprite animations to be used.
	std::map<string, Animation> animations;

protected:

	void SetUV(uint32 idx, vec2 value);

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
