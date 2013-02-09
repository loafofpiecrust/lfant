/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
 *	Created: 2012-08-18 by Taylor Snead
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *	http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 *
 ******************************************************************************/
#include "sfRigidBody.hpp"

// External

// Internal

namespace sfs
{

sfRigidBody::sfRigidBody(const btRigidBodyConstructionInfo& constructionInfo) :
	btRigidBody(constructionInfo)
{
}

sfRigidBody::~sfRigidBody()
{
}

void sfRigidBody::setupRigidBody(
	const btRigidBody::btRigidBodyConstructionInfo& constructionInfo)
{
	btRigidBody::setupRigidBody(constructionInfo);
	m_gravityMult = 1.0f;
}

void sfRigidBody::applyPointGravity(btVector3 point, const btScalar force)
{
	if(force == 0)
	{
		return;
	}
	/*
	 // @todo Change to use center of mass instead.
	 // vec3 dirvec = normalize( point - transform->position );
	 //btVector3 dir = btVector3( normalize( point - transform->position ) );
	 // scalar dist = distance( point, transform->position );

	 // Use a version of the gravitational formula:
	 //
	 // F = (GMm)/d^2
	 //
	 // If G and M are fixed (rolled into m_gravityConstant), then F = m_gravityConstant *	m / d^2
	 // i.e. the force is proportional both to the mass, and the inverse distance squared.
	 // scalar magnitude = (0.00000000006673 *	mass *	mMass) / (dist *	dist);

	 // if (magnitude < 0.3f)
	 //{
	 //	return;
	 //}

	 // Apply the gravity force.
	 // applyCentralForce( magnitude *	dir );

	 // if (dist < 8.0f)
	 //{
	 //	btRigidBody::setDamping( -0.1f, 0.0f );
	 //}
	 //else
	 //{
	 //	btRigidBody::setDamping( 0.0f, 0.0f );
	 //}
	 */
}

}
