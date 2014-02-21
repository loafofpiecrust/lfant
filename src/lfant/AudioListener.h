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

#include <lfant/Audio.h>

#include <lfant/Component.h>

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
class AudioListener : public Component
{
//	DECLARE_TYPE(Component, AudioListener)
public:
	float gain;
	vec3 velocity;

protected:

private:

};

/// @}
/// @}

} /* namespace lfant */
