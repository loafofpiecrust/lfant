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

// Internal
#include <lfant/Component.h>
#include <lfant/Range.h>
#include <lfant/Sound.h>

namespace lfant
{
/** @addtogroup Game
 *	@{
 */
/** @addtogroup Audio
 *	@{
 */

/**
 *
 */
class AudioSource : public Component
{
//	DECLARE_TYPE(Component, AudioSource)
public:
	AudioSource();
	virtual ~AudioSource();

	virtual void Init();

	Sound* AddSound(string name);
	Sound* GetSound(string name);

protected:

	void OnSetPosition();
	void OnSetRotation();
	void OnSetVelocity(vec3 vel);

	std::deque<std::shared_ptr<Sound>> sounds;

private:
	string inputFile = "";
};
}
