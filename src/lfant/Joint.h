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

class btTypedConstraint;

namespace lfant
{

class Rigidbody;

/**	@addtogroup Game
 *	@{
 */
/** @addtogroup Physics
 *	@{
 */

/**
 *
 */
class Joint : public Component
{
	friend class Physics;

public:
	virtual ~Joint();

	Rigidbody* rigidbody;
	Rigidbody* connectedBody;

protected:
	Joint();

	virtual void Init();

private:

	virtual btTypedConstraint* GetConstraint() = 0;
	virtual void SetConstraint(btTypedConstraint* con) = 0;
};

/** @} */
/** @} */
}
