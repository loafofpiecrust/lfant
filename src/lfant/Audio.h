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
#include <lfant/Subsystem.h>

struct ALCdevice_struct;
struct ALCcontext_struct;


namespace lfant {

class Halves;
class Sound;

/**	@addtogroup Game
 *	@{
 */
/** @addtogroup Audio
 *	@{
 */

/**
 *
 */
class Audio : public Subsystem
{
	friend class AudioSource;
public:
	Audio();
	virtual ~Audio();

	virtual void Init();

	/**
	 *	Plays a 2D sound at global volume.
	 *	@param file The audio file to load and play.
	 */
//	 PlaySound(string file, bool loop = false);

	uint8_t maxChannels = 10;

protected:
	virtual void Update();
	virtual void Deinit();

private:
	std::deque<std::shared_ptr<Sound>> sounds;

	ALCdevice_struct* device;
	ALCcontext_struct* context;
};

}
