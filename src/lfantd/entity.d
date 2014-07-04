
module lfant.entity;

// External
import std.c.string;
import std.container;
import std.stdio;
import std.algorithm;
import std.array;
import std.range;

import dcollections.ArrayList;

// Internal
import lfant.gameobject;
import lfant.game;
import lfant.component;
import lfant.scene;
import lfant.transform;

/** @addtogroup Game
 *	@{
 */
/** @addtogroup Core
 *	@{
 */

/**
 *	This class is the basis for all Entities.
 *		Handles basic object and entity duties, like initialization,
 *		updating every frame, then cleaning up on destruction. Cloning is also
 *		supported by default. Entities can have functionality added to them
 *		by adding Components to their list of Component instances. Those
 *		are modules of functionality and data ready to be added onto an Entity.
 *		This prefers aggregation over inheritance.
 *	@todo
 *		Organize...
 */
final class Entity : GameObject
{
public:
	this(Scene scene)
	{
		this.scene = scene;
		transform = addComponent!(Transform)();
	}
	~this()
	{
		writeln("entity");
	}

	static void scriptBind()
	{
	}

	Entity addChild(string name = "")
	{
		Entity ent = new Entity(scene);
		ent.name = name;
		addChild(ent);
	//	scene.triggerEvent("AddEntity", ent);
		return ent;
	}
	void removeChild(Entity ent)
	{
	//	auto elem = find(children[], ent);
	//	children.linearRemove(take(elem, 1));
		for(uint i = 0; i < children.length; ++i)
		{
			if(children[i] is ent)
			{
			//	children.linearRemove(children[i..i+1]);
				children.remove(children[i..i+1]);
				break;
			}
		}
	}

	private void addComponent()(Component comp)
	{
		components ~= comp;
	}
	/**
	 *	Adds a new component by instancing the given type.
	 *	@tparam C The class of component to add.
	 */
	C addComponent(C)()
	{
		C comp = getComponent!(C)();
		if(comp)
		{
			return comp;
		}
		else
		{
			comp = new C();
			addComponent(comp);
		//	TriggerEvent("OnSetComponent"+type::Descope(type::Name(comp)), comp);
			return comp;
		}
	}

	/**
	 *	Removes a component from this Entity
	 *	@tparam C The type of component to remove.
	 */
	void removeComponent(C)()
	{
		for(uint i = 0; i < components.length; ++i)
		{
			ref auto comp = components[i];
			if(typeid(C) == typeid(comp))
			{
				components.linearRemove(i);
				break;
			}
		}
	}

	void removeComponent(string type)
	{
	//	components.linearRemove(take(components.find(
		for(uint i = 0; i < components.length; ++i)
		{
			auto comp = components[i];
			if(typeid(comp).toString() == type)
			{
			//	components.linearRemove(components[i .. i+1]);
				components.remove(components[i..i+1]);
			}
		}
	}

/*	auto GetComponent(C)()
	{
	//	string type = type::Name<C>();
		C* result = null;
		foreach(comp; components)
		{
			result = cast(C*)(comp.get());
			if(result)
			{
				return result;
			}
		}
		return null;
	}*/

	C getComponent(C)()
	{
		foreach(comp; components)
		{
			if(typeid(comp) == typeid(C))
			{
				return cast(C)comp;
			}
		}
		return null;
	}

	Component getComponent(string type)
	{
		foreach(comp; components)
		{
			if(typeid(comp).toString() == type)
			{
				return comp;
			}
		}
		return null;
	}

	Entity getChild(string name, bool recursive = false)
	{
		foreach(child; children)
		{
			if(child.name == name)
			{
				return child;
			}
		}
		return null;
	}
	
	Entity getChildById(uint id) const
	{
		return null;
	}

//	const std::deque<ptr<Component>>& GetComponents() const;
//	const std::deque<ptr<Entity> >& GetChildren() const;

//	Entity* SpawnChild();

/*	bool HasTag(string tag) const;

	uint GetId() const { return id; }

	uint GetLayer() const;
	void SetLayer(uint layer);

	Entity GetParent() const;
	Entity GetParent(uint height) const;
	void Reparent(Entity* ent);

	string GetName() const;
	void Rename(string name);

	void Enable(bool on);
	bool IsEnabled() const;*/

	@property
	{
		Entity parent()
		{
			return _parent;
		}
		void parent(Entity ent)
		{
			parent.removeChild(this, false);
			if(ent)
			{
				ent.addChild(this);
			}
			else
			{
				scene.root.addChild(this);
			}
		}
	
		Transform transform()
		{
			return _transform;
		}
		private void transform(Transform trans)
		{
			_transform = trans;
		}
		
		override Game game()
		{
			return scene.game;
		}
	}

//	Transform* transform;

	bool enabled = true;
//	std::vector<string> tags;
	Array!(string) tags;
	float lifetime = 0.0;

//private:

//	void Init();
	override void update(in float delta)
	{
		if(lifetime > 0.0f)
		{
		//	lifetime -= GetGame().time.deltaTime;
			if(lifetime <= 0.0f)
			{
				wantsToDie = true;
			}
		}
	
		foreach(ref comp; components)
		{
			if(comp.enabled)
			{
				comp.update(delta);
			}
		}
	
		foreach(ref child; children)
		{
			if(child.enabled)
			{
				child.update(delta);
			}
		}
	}

	override void render()
	{

	}
	override void deinit()
	{
	/*	foreach(ent; children)
		{
			ent._parent = null;
		}*/
		children.clear();
		components.clear();
	}

	void destroyDeadChildren()
	{

	}

	/// @todo Make public?
	void addChild(Entity ent)
	{
		ent.parent = this;
		ent.scene = this.scene;
		children ~= ent;
	//	children.insertFront(ent);
	}
	void removeChild(Entity ent, bool destroy)
	{

	}

	void triggerEventWithChildren(P...)(string name, P args)
	{
		triggerEvent(name, args);
		foreach(c; children)
		{
			if(!c.enabled) continue;
			c.triggerEventWithChildren(name, args);
		}
	}

	void triggerEventWithParent(P...)(string name, P args)
	{
		triggerEvent(name, args);
		if(parent && parent.enabled)
		{
			parent.triggerEvent(name, args);
		}
	}

//	SList!(Component) components;
//	SList!(Entity) children;
//	Component[] components;
//	Entity[] children;
	ArrayList!Entity children = new ArrayList!Entity;
	ArrayList!Component components = new ArrayList!Component;

	string name = "root";

	bool wantsToDie = false;

	/// Scene-unique identifier.
	uint id = 0;

	/// The layer of this entity for primarily display filtering
	uint layer = 0;

	Scene scene = null;

	// properties
	Entity _parent;
	Transform _transform;
};

/** @} */
/** @} */

