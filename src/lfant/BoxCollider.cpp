/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2012-11-01 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/

#include <lfant/BoxCollider.h>

// External
#include <btBulletCollisionCommon.h>

// Internal

namespace lfant
{

IMPLEMENT_SUBTYPE(Component, BoxCollider, Collider)

void BoxCollider::Init()
{
	shape = new btBoxShape(vec3_cast<btVector3>(size));
	Collider::Init();
}

btCollisionShape *BoxCollider::GetShape()
{
	return shape;
}

}
