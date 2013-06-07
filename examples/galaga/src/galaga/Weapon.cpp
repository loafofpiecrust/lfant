/******************************************************************************
 *
 *	LFANT Source
 *	Copyright (C) 2012-2013 by LazyFox Studios
 *	Created: 2013-06-06 by Taylor Snead
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

#include <galaga/Weapon.h>

// internal
#include <lfant/Game.h>
#include <lfant/Time.h>

// external

namespace lfant {
namespace galaga {

Weapon::Weapon()
{
}

Weapon::~Weapon()
{
}

void Weapon::Load(Properties* prop)
{
	Component::Load(prop);

	prop->Get("currentAmmo", currentAmmo);
	prop->Get("maxAmmo", maxAmmo);
	prop->Get("shotCost", shotCost);
	prop->Get("fireRate", fireRate);
	prop->Get("reloadTime", reloadTime);
	prop->Get("meleeDamage", meleeDamage);
}

void Weapon::Save(Properties* prop)
{
	Component::Save(prop);

	prop->Set("currentAmmo", currentAmmo);
	prop->Set("maxAmmo", maxAmmo);
	prop->Set("shotCost", shotCost);
	prop->Set("fireRate", fireRate);
	prop->Set("reloadTime", reloadTime);
	prop->Set("meleeDamage", meleeDamage);
}

void Weapon::Init()
{
	ConnectEvent(SENDER(owner, UseItem), RECEIVER(this, Fire));
	ConnectEvent(SENDER(this, Reload), RECEIVER(this, EndReload));
}

void Weapon::Update()
{
	Component::Update();

	if(lastFire < 1/fireRate)
	{
		lastFire += game->time->deltaTime;
	}
}

void Weapon::Fire()
{
	if(CanFire())
	{
		// @fixme Put above or keep in here? (Pulling empty trigger while reload cancels reload?)
		if(reloading)
		{
			reloading = false;
			CancelTimer("Reload");
			TriggerEvent("StopAnimation");
		}

		currentAmmo -= shotCost;
		lastFire = 0.0f;

		// @todo Firing code?
		TriggerEvent("PlayAnimation", "Fire");
	}
}

void Weapon::Reload()
{
	reloading = true;
	SetTimer("Reload", reloadTime);
	TriggerEvent("PlayAnimation", "Reload", reloadTime);
}

void Weapon::EndReload()
{
	reloading = false;
}

bool Weapon::CanFire()
{
	return lastFire < fireTime;
}

}
}