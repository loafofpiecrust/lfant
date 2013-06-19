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
#include <lfant/Console.h>
#include <lfant/Transform.h>
#include <lfant/Scene.h>
#include <lfant/Input.h>
#include <lfant/util/lexical_cast.h>

// external

namespace lfant {
namespace galaga {

IMPLEMENT_COMP(Weapon)

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
	prop->Get("bulletSpeed", bulletSpeed);
	prop->Get("maxAmmo", maxAmmo);
	prop->Get("shotCost", shotCost);
	prop->Get("projectiles", projectiles);
	prop->Get("fireRate", fireRate);
	prop->Get("reloadTime", reloadTime);
	prop->Get("meleeDamage", meleeDamage);
	prop->Get("projectilePath", projectilePath);
	prop->Get("automatic", automatic);
	prop->Get("direction", direction);
}

void Weapon::Save(Properties* prop)
{
	Component::Save(prop);

	prop->Set("currentAmmo", currentAmmo);
	prop->Set("bulletSpeed", bulletSpeed);
	prop->Set("maxAmmo", maxAmmo);
	prop->Set("shotCost", shotCost);
	prop->Set("projectiles", projectiles);
	prop->Set("fireRate", fireRate);
	prop->Set("reloadTime", reloadTime);
	prop->Set("meleeDamage", meleeDamage);
	prop->Set("projectilePath", projectilePath);
	prop->Set("automatic", automatic);
	prop->Set("direction", direction);
}

void Weapon::Init()
{
	Component::Init();
	
	ConnectEvent(SENDER(owner, Use), RECEIVER(this, Fire));
	ConnectEvent(SENDER(owner, EndUse), RECEIVER(this, EndFire));
	ConnectEvent(SENDER(this, Reload), RECEIVER(this, EndReload));

	lastFire = 1/fireRate;
}

void Weapon::Update()
{
	Component::Update();

	if(lastFire < 1/fireRate)
	{
		lastFire += game->time->deltaTime;
	}
	else if(!released && automatic)
	{
		Fire();
	}

	if(game->input->GetButtonDown("ShowLoc"))
	{
		Log("Weapon pos: ", lexical_cast<string>(owner->transform->GetPosition()));
	//	owner->transform->SetPosition(vec3(0,0,5));
	}
}

void Weapon::Fire(byte mode)
{
	Log("Starting fire process");
	if(mode == 0 && CanFire())
	{
		if(currentAmmo <= 0)
		{
			Reload();
		}
		Log("Able to fire.");
		// @fixme Put above or keep in here? (Pulling empty trigger while reload cancels reload?)
		if(reloading)
		{
			Log("Cancel reload");
			reloading = false;
			CancelTimer("Reload");
			TriggerEvent("StopAnimation");
		}

		currentAmmo -= shotCost;
		lastFire = 0.0f;

		for(uint i = 0; i < projectiles; ++i)
		{
			Entity* ent = game->scene->Spawn();
			ent->SetLayer(owner->GetLayer());
			ent->transform->SetPosition(owner->transform->GetWorldPosition());
			ent->transform->SetRotation(owner->transform->GetWorldRotation());
			ent->LoadFile(projectilePath);
			vec3 final = owner->transform->GetDirection() * direction * bulletSpeed / game->time->deltaTime;
			Log("Weapon bullet speed firing, ", bulletSpeed, ", final accel of ", lexical_cast<string>(final));
			ent->TriggerEvent("Accelerate", final);
		}

		Log("Finish off firing, ammo: ", currentAmmo);

		// @todo Firing code?
		TriggerEvent("PlayAnimation", "Fire");

		released = false;
	}
	else if(mode == 1)
	{
		Reload();
	}
}

void Weapon::EndFire()
{
	Log("Weapon ending firing.");
	released = true;
	// Anything else?
}

void Weapon::Reload()
{
	if(reloading)
		return;

	if(ammoPool <= 0)
	{
		// @todo Something here?
		return;
	}

	Log("Beginning reload");
	reloading = true;
	SetTimer("Reload", reloadTime);
	TriggerEvent("PlayAnimation", "Reload", reloadTime);
}

void Weapon::EndReload()
{
	Log("Finishing reload");
	ammoPool -= maxAmmo - currentAmmo;
	currentAmmo = maxAmmo;
	reloading = false;
}

bool Weapon::CanFire()
{
	return lastFire >= 1/fireRate && (released || automatic);
}

}
}