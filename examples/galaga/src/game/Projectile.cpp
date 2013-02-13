/******************************************************************************
 *
 * ShadowFox Engine Source
 * Copyright (C) 2012-2013 by ShadowFox Studios
 * Created: 2012-09-09 by Taylor Snead
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#include "Projectile.hpp"

// External

// Internal
#include "Mesh.hpp"
#include "Sprite.hpp"

using namespace sfs;

Projectile::Projectile()
{
}

Projectile::~Projectile()
{
}

void Projectile::Init()
{
	AddComponent<Sprite>();
}

void Projectile::Impact( KillParams death )
{

}

void Projectile::Penetrate( float thickness )
{

}

void Projectile::Launch( const vec3 pos, const vec3 dir, const vec3 velocity, float speedScale )
{

}

void Projectile::Ricochet()
{

}
