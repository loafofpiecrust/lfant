/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
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

/** @addtogroup Game @{ */
/** @addtogroup Core @{ */

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

	Entity* GetRoot() { return root; }

	/** Returns pointers to all entities with a certain tag.
	 *	@param tag Tag to search for.
	 *	@return All entities with tag.
	 */
	std::deque<Entity*> GetEntities(string tag) const;
	std::deque<Entity*> GetLayer(string layer) const;

	/** Saves the scene to a file.
	 *	@param file File to save to, without extension.
	 */
	virtual void Save(Properties *prop) const;

	/** Loads the scene from a file.
	 *	@param file File to load from, without extension.
	 */
	virtual void Load(Properties* prop);

	virtual void LoadFile(string path);

	void Clear();

	Camera* mainCamera = nullptr;
	string name = "Scene";
	string currentFile = "";

protected:

	/** Removes an Entity from the Scene, only called by that Entity
	 *	just before destruction.
	 *  @param name Name of the Entity to remove.
	 */
	void RemoveEntity(Entity* ent, bool destroy = true);

	uint32_t GenerateEntityId();

	ptr<Entity> root;
//	deque< ptr<Entity> > entities;
	uint32_t currentId = 0;

private:

};

/// @}
/// @}

} /* namespace lfant */
