/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013, by ShadowFox Studios
 *	Created: 2012-08-13 by Taylor Snead
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *		http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 *
 ******************************************************************************/
#pragma once
#include "stdafx.hpp"

// External

// Internal

namespace sfs
{
class Audio
{
public:
	Audio();
	virtual ~Audio();

	/**	Plays a sound at the origin to the global listener at the origin.
	 *	@details Use for things like music, but if music is to be heard by just
	 *		a single entity, it's recommended to simply attach an AudioSource to
	 *		that entity, and play the music only to it's AudioListener.
	 *	@param file The audio file to load and play.
	 */
	void PlaySound(string file);

protected:
	virtual void Init();
	virtual void Update();

private:
	uint32_t source;
	uint32_t listener;
	uint32_t buffer;
};
}
