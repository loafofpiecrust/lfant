/******************************************************************************
 *
 * ShadowFox Engine Source
 * Copyright (C) 2012-2013 by ShadowFox Studios
 * Created: 2012-09-02 by Taylor Snead
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
#pragma once
#include "stdafx.hpp"

// External

// Internal
#include "Subsystem.hpp"

namespace sfs
{
class Entity;
class Camera;

/** @addtogroup Engine
 *	@{
 */
/** @addtogroup Subsystems
 *	@{
 */

/**
 *	@details
 *
 *	@todo
 *
 */
class Scene : public Subsystem
{
	friend class Engine;

public:
	Scene();
	virtual ~Scene();

	virtual void Init();
	virtual void Update();
	virtual void OnDestroy();

	/**
	 *	Gets the first entity with a certain name.
	 */
	Entity* GetEntity(string name, bool recursive = true);

	/**
	 *	Gets the first entity with a certain tag.
	 */
	Entity* GetEntityByTag(string tag);

	/**
	 *	Gets an entity by its unique id.
	 */
	Entity* GetEntityById(uint32_t id);

	/**
	 *	Gets the first entity in a given layer.
	 *	@todo
	 *		Not sure whether layer should be uint16 or string.
	 */
	Entity* GetEntityByLayer(string layer);

	/**
	 *	Saves the scene to a specific file.
	 */
	void Save(string file = "");

	Entity* Spawn(string name, Entity* parent = nullptr, vec3 pos = vec3(0), vec3 rot = vec3(0), vec3 scale = vec3(0));

	vector<Entity*> entities;
	Camera* mainCamera = nullptr;
	string name = "Scene";

protected:

private:

};

/// @}
/// @}

} /* namespace sfs */
