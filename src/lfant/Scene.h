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
	virtual void Render();
	virtual void Deinit();
	virtual void FixedUpdate();

	/**
	 *	Gets the first entity with a certain name.
	 */
	Entity* GetEntity(string name, bool recursive = true) const;

	/**
	 *	Gets the first entity with a certain tag.
	 */
	Entity* GetEntityByTag(string tag) const;

	/**
	 *	Gets an entity by its unique id.
	 */
	Entity* GetEntityById(uint32 id, bool recursive = true) const;

	/**
	 *	Gets the first entity in a given layer.
	 *	@todo
	 *		Not sure whether layer should be uint16 or string.
	 */
	Entity* GetEntityByLayer(string layer) const;

	/**
	 *	Returns pointers to all entities with a certain tag.
	 *	@param tag Tag to search for.
	 *	@return All entities with tag.
	 */
	deque<Entity*> GetEntities(string tag) const;

	const deque<ptr<Entity>>& GetEntities() const;

	deque<Entity*> GetLayer(string layer) const;

	/**
	 *	Saves the scene to a file.
	 *
	 *	@param file File to save to, without extension.
	 */
	virtual void Save(Properties *prop) const;

	/**
	 *	Loads the scene from a file.
	 *
	 *	@param file File to load from, without extension.
	 */
	virtual void Load(Properties* prop);

	void Clear();

	/**
	 *	Spawns an entity in this Scene. Use this for any creation
	 *	of an entity. Never try to use the Entity constructor(s).
	 *
	 *	@param name Name of new Entity.
	 *	@param parent Parent entity, defaults to nullptr.
	 *	@return The new entity.
	 */
	Entity* Spawn(Entity* parent = nullptr, string name = "Entity");
	Entity* SpawnAndLoad(Properties* prop, Entity* parent = nullptr, string name = "Entity");

	Camera* mainCamera = nullptr;
	string name = "Scene";

protected:

	/**
	 *	Removes an Entity from the Scene, only called by that Entity
	 *	just before destruction.
	 *	@param name Name of the Entity to remove.
	 */
	void RemoveEntity(Entity* ent, bool destroy = true);

	void AddEntity(Entity* ent);

	uint32_t GenerateEntityId();

	deque< ptr<Entity> > entities;
	uint32_t currentId = 0;

private:

};

/// @}
/// @}

} /* namespace lfant */
