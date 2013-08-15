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
#pragma once

// External

// Internal
#include <galaga/Inventory.h>

namespace lfant {
namespace galaga {

/** @addtogroup Galaga
 *	@{
 */
/** @addtogroup Entities
 *	@{
 */

/**
 *
 */
class Weapon : public Item
{
	DECLARE_COMP(Weapon)
public:

	class Part : public Item
	{
		DECLARE_COMP(Part)
		friend class Weapon;
	public:
		enum class Type : byte
		{
			Body = 1,
			Grip,
			Stock,
			Barrel,
			Muzzle,
			Ammo,
			Sight
		};

		Part();
		~Part();

		virtual void Load(Properties* prop);
		virtual void Save(Properties* prop);

	protected:
		Type type = Type::Body;
	//	Weapon::Type weaponType = Weapon::Type::Pistol;

		float meleeDamage = 1.0f;
		float bulletSpeed = 0.0f;
		float spread = 0.0f;
		float fireRate = 0.0f;

		// Ammo
		uint32_t ammo = 0;
		string projectilePath = "entities/items/projectiles/Bullet.ent";

		vec3 attachmentPoint = vec3(0);

	private:
	};

	class Body : public Part
	{
		DECLARE_COMP(Body)
	public:
		Body();
		virtual ~Body() {}

		virtual void Load(Properties* prop);
		virtual void Save(Properties* prop);

		bool automatic = false;
		uint32_t projectiles = 1;
		uint32_t shotCost = 1;

		map<Part::Type, vec3> attachmentPoints;
	};

	Weapon();
	virtual ~Weapon();

	virtual void Load(Properties* prop);
	virtual void Save(Properties* prop);

	virtual void Init();
	virtual void Update();

	void Fire(byte mode = 0);
	void Reload();
	void EndReload();

	bool CanFire();
	bool CanReload();

	void Assemble(const deque<Part*>& partList);
	void Disassemble();
	void AddPart(Part* part);
	void RemovePart(Part* part);

protected:
	void EndFire();

	uint32_t currentAmmo = 0;
	uint32_t maxAmmo = 0;
	uint32_t ammoPool = 100;
	uint32_t shotCost = 1;
	uint32_t projectiles = 1;
	float fireRate = 0.0f;
	float reloadTime = 0.0f;
	float meleeDamage = 0.0f;
	float bulletSpeed = 0.0f;

	float spread = 0.0f;
	float currentSpread = 0.0f;

	float recoil = 0.0f;
	float currentRecoil = 0.0f;

	float lastFire = 0.0f;

	bool reloading = false;
	bool automatic = false;
	bool released = true;

	vec3 direction = vec3(1);

	string projectilePath = "entities/items/projectiles/Bullet.ent";
	ptr<Properties> projectile = new Properties;

	Body* body;
	deque<Part*> parts;


private:

};

/// @}
/// @}
}
}
