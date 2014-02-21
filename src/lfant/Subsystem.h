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
#include <lfant/Object.h>

namespace lfant
{
/** @addtogroup Game
 *	@{
 */
/** @addtogroup Core
 *	@{
 */

/**
 *
 */
class Subsystem : public Object
{
public:
	virtual void Destroy();
protected:
	Subsystem();
	virtual ~Subsystem();

	virtual void Init();
};

/// @}
/// @}

} /* namespace lfant */
