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
IMPLEMENT_COMP(Weapon::Part)
IMPLEMENT_COMP(Weapon::Body)

Weapon::Weapon()
{
}

Weapon::~Weapon()
{
}

void Weapon::Load(Properties* prop)
{
	Component::Load(prop);

	deque<string> partNames;
	prop->Get("parts", partNames);
	for(auto& pname : partNames)
	{
		Properties* pprop = new Properties;
		pprop->LoadFile(pname);

		Part* part = nullptr;
		if(pprop->Get<byte>("type") == (byte)Part::Type::Body)
		{
			part = new Body;
		}
		else
		{
			part = new Part;
		}

		part->Load(pprop);
		AddPart(part);
	}

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

	projectile->LoadFile(projectilePath);
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
	
	ConnectEvent(SENDER(this, Use), RECEIVER(this, Fire));
	ConnectEvent(SENDER(this, EndUse), RECEIVER(this, EndFire));
	ConnectEvent(SENDER(this, Reload), RECEIVER(this, Reload));
	ConnectEvent(SENDER(this, EndReload), RECEIVER(this, EndReload));

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
			ent->Load(projectile);
			vec3 final = owner->transform->GetDirection() * direction * bulletSpeed * 1000.0f;
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


void Weapon::Assemble(const deque<Weapon::Part*>& partList)
{
	parts[0]->inventory->AddItem(this);

	for(auto& part : partList)
	{
		AddPart(part);
	}

}

void Weapon::AddPart(Weapon::Part* part)
{
	if(part->inventory)
	{
		part->inventory->RemoveItem(part);
	}
	if(part->owner)
	{
		part->owner->SetParent(owner);
	}

	if(part->type == Part::Type::Body)
	{
		Body* body = dynamic_cast<Body*>(part);
		automatic = body->automatic;
		shotCost = body->shotCost;
		projectiles = body->projectiles;
		this->body = body;
	}
	else
	{
		parts.push_back(part);
	}

	if(part->type == Part::Type::Ammo)
	{
		projectilePath = part->projectilePath;
		reloadTime += part->mass*2;
	}
	else
	{
		reloadTime += part->mass/2;
	}

	mass += part->mass;
	maxAmmo += part->ammo;
	fireRate += part->fireRate;
	meleeDamage += part->meleeDamage;
	bulletSpeed += part->bulletSpeed;
	spread += part->spread;
	recoil += 1/part->mass * part->bulletSpeed/100;
}

void Weapon::Disassemble()
{
	for(auto& p : parts)
	{
		if(inventory)
		{
			inventory->AddItem(p);
		}
		if(owner)
		{
			p->owner->SetParent(owner->GetParent());
		}
	}
	owner->Destroy();
}

void Weapon::RemovePart(Weapon::Part* part)
{
	if(part->type == Part::Type::Body)
	{
		Disassemble();
		return;
	}

	if(inventory)
	{
		inventory->AddItem(part);
	}
	if(owner)
	{
		part->owner->SetParent(owner->GetParent());
	}

	for(uint i = 0; i < parts.size(); ++i)
	{
		if(parts[i] == part)
		{
			parts.erase(parts.begin()+i);
			break;
		}
	}

	mass -= part->mass;
	maxAmmo -= part->ammo;
	meleeDamage -= part->meleeDamage;
	bulletSpeed -= part->bulletSpeed;
	spread -= part->spread;
	recoil -= 1/part->mass * part->bulletSpeed/100;
}


Weapon::Part::Part()
{
}

Weapon::Part::~Part()
{
}

void Weapon::Part::Load(Properties* prop)
{
	Item::Load(prop);

	type = (Weapon::Part::Type)prop->Get<byte>("type");
	prop->Get("meleeDamage", meleeDamage);
	prop->Get("bulletSpeed", bulletSpeed);
	prop->Get("spread", spread);
	prop->Get("fireRate", fireRate);
	prop->Get("ammo", ammo);
	prop->Get("projectilePath", projectilePath);
	prop->Get("attachmentPoint", attachmentPoint);
}

void Weapon::Part::Save(Properties* prop)
{
	Item::Save(prop);

	prop->Set("type", (byte)type);
	prop->Set("meleeDamage", meleeDamage);
	prop->Set("bulletSpeed", bulletSpeed);
	prop->Set("spread", spread);
	prop->Set("fireRate", fireRate);
	prop->Set("ammo", ammo);
	prop->Set("projectilePath", projectilePath);
	prop->Set("attachmentPoint", attachmentPoint);
}


Weapon::Body::Body()
{
	type = Weapon::Part::Type::Body;
}

void Weapon::Body::Load(Properties* prop)
{
	Part::Load(prop);

	prop->Get("automatic", automatic);
	prop->Get("projectiles", projectiles);
	prop->Get("shotCost", shotCost);

	for(auto& patt : prop->GetChildren("attachment"))
	{
		attachmentPoints[(Weapon::Part::Type)patt->Get<byte>("type")] = patt->Get<vec3>("position");
	}
}

void Weapon::Body::Save(Properties* prop)
{
	Part::Save(prop);

	prop->Set("automatic", automatic);
	prop->Set("projectiles", projectiles);
	prop->Set("shotCost", shotCost);

	for(auto& att : attachmentPoints)
	{
		Properties* patt = prop->AddChild("attachment");
		patt->Set("type", (byte)att.first);
		patt->Set("position", att.second);
	}
}

}
}