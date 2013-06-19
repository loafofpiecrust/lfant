/******************************************************************************
 *
 *	LFANT Source
 *	Copyright (C) 2012-2013 by LazyFox Studios
 *	Created: 2013-06-12 by Taylor Snead
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

#include <galaga/Projectile.h>

// Internal
#include <lfant/Rigidbody.h>
#include <lfant/Physics.h>
#include <lfant/Console.h>

// External

namespace lfant {
namespace galaga {

IMPLEMENT_COMP(Projectile)

Projectile::Projectile()
{

}

Projectile::~Projectile()
{

}

void Projectile::Init()
{
	Component::Init();

	ConnectEvent(SENDER(owner, CollideEnter), RECEIVER(this, OnCollideEnter));
}

void Projectile::OnCollideEnter(Rigidbody* rb, Collision* col)
{
	if(!col->other || !rb)
	{
		return;
	}
	Log("Collision! ", col->other->owner->GetLayer());
	if((owner->GetLayer() == "Player" && col->other->owner->GetLayer() == "Enemy") || (owner->GetLayer() == "Enemy" && col->other->owner->GetLayer() == "Player"))
	{
		// Call some destruction thingy?
		col->other->owner->Destroy();
	}
}

}
}