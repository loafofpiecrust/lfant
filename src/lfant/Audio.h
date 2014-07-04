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

namespace irrklang
{
class ISoundEngine;
}

namespace lfant
{

class Sound;
class AudioListener;

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
	friend class AudioListener;
public:
	Audio(Game* game);
	virtual ~Audio();

	virtual void Init();

	void PlaySound(string file, bool loop = false);

	AudioListener* mainListener;
	uint8 maxChannels = 10;

protected:
	virtual void Update();
	virtual void Deinit();

private:
	std::deque<std::shared_ptr<Sound>> sounds;
//	irrklang::ISoundEngine* engine = nullptr;
};

}
