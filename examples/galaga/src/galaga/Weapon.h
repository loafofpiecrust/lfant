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
#include <lfant/Component.h>

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
class Weapon : public lfant::Component
{
	DECLARE_COMP(Weapon)
public:
	Weapon();
	virtual ~Weapon();

	void Load(Properties* prop);
	void Save(Properties* prop);

	virtual void Init();
	virtual void Update();

	void Fire(byte mode = 0);
	void Reload();
	void EndReload();

	bool CanFire();
	bool CanReload();

protected:
	void EndFire();

	uint32_t currentAmmo = 25;
	uint32_t maxAmmo = 25;
	uint32_t ammoPool = 100;
	uint32_t shotCost = 1;
	uint32_t projectiles = 1;
	float fireRate = 10.0f;
	float reloadTime = 1.0f;
	float meleeDamage = 10.0f;
	float bulletSpeed = 110.0f;

	float lastFire = 0.0f;

	bool reloading = false;
	bool automatic = false;
	bool released = true;

	vec3 direction = vec3(1);

	string projectilePath = "entities/items/projectiles/Bullet.ent";


private:

};

/// @}
/// @}
}
}
