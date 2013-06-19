/******************************************************************************
*
* LFANT Source
* Copyright (C) 2012-2013 by LazyFox Studios
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
#include <lfant/stdafx.h>

// Internal
#include <lfant/ptr.h>
#include <lfant/Subsystem.h>

// External
#include <list>
#include <forward_list>

namespace lfant
{
class Entity;
class Camera;

/** @addtogroup Game
 *	@{
 */
/** @addtogroup Core
 *	@{
 */

/**
 *
 */
class Scene : public Subsystem
{
	friend class Game;
	friend class Entity;

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
	Entity* GetEntityById(uint32 id, bool recursive = true);

	/**
	 *	Gets the first entity in a given layer.
	 *	@todo
	 *		Not sure whether layer should be uint16 or string.
	 */
	Entity* GetEntityByLayer(string layer);

	/**
	 *	Returns pointers to all entities with a certain tag.
	 *	@param tag Tag to search for.
	 *	@return All entities with tag.
	 */
	deque<Entity*> GetEntities(string tag);

	deque<Entity*> GetLayer(string layer);

	/**
	 *	Saves the scene to a file.
	 *
	 *	@param file File to save to, without extension.
	 */
	void Save(Properties *prop);

	/**
	 *	Loads the scene from a file.
	 *
	 *	@param file File to load from, without extension.
	 */
	void Load(Properties* prop);

	/**
	 *	Spawns an entity in this Scene. Use this for any creation
	 *	of an entity. Never try to use the Entity constructor(s).
	 *
	 *	@param name Name of new Entity.
	 *	@param parent Parent entity, defaults to nullptr.
	 *	@param pos Position, defaults to the origin.
	 *	@param rot Rotation, defaults to 0 degrees xyz.
	 *	@param scale Scale, defaults to x1.
	 *	@return The new entity.
	 */
	Entity* Spawn(string name = "Entity", Entity* parent = nullptr);
	Entity* SpawnAndLoad(Properties* prop, string name = "Entity", Entity* parent = nullptr);

	Camera* mainCamera = nullptr;
	string name = "Scene";

protected:

	/**
	 *	Removes an Entity from the Scene, only called by that Entity
	 *	just before destruction.
	 *	@param name Name of the Entity to remove.
	 */
	void RemoveEntity(Entity* ent);

	uint32_t GenerateEntityId();

	deque< ptr<Entity, Object::Delete> > entities;
	uint32_t currentId = 0;

private:

};

/// @}
/// @}

} /* namespace lfant */
