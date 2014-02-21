/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2012-11-02 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/

#include <lfant/Joint.h>

// External
#include <btBulletDynamicsCommon.h>

// Internal
#include <lfant/TypeInfo.h>

#include <lfant/Entity.h>
#include <lfant/Rigidbody.h>

namespace lfant
{

Joint::Joint()
{
}

Joint::~Joint()
{
}

/*******************************************************************************
*
*		Game Loop
*
*******************************************************************************/

void Joint::Init()
{
	if(Rigidbody* rb = owner->GetComponent<Rigidbody>())
	{
		rigidbody = rb;
	}

	ConnectEvent(SENDER(owner, SetComponentRigidbody), &rigidbody);
}

}
