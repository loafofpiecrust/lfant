
module lfant.scene;

import std.array;
import std.stdio;

import lfant.entity;
import lfant.game;
import lfant.subsystem;
import lfant.properties;

class Scene : Subsystem
{
public:
	this(Game game)
	{
		super(game);
	}
	~this()
	{
		writeln("scene");
	}

	static void scriptBind();

	override void initialize()
	{
		root = new Entity(this);
		if(!game.defaultScene.empty)
		{
			game.log("Loading scene '"~game.defaultScene~"'...");
			loadFile(game.defaultScene);
		}
	}
	override void update(in float delta)
	{
		root.update(delta);
		root.destroyDeadChildren();
	}
	override void render()
	{
		root.render();
	}
	override void deinit()
	{
		root.deinit();
		super.deinit();
	}
	
	override void serialize(Properties prop)
	{
		currentId = 0;
		if(prop.mode == Properties.Mode.Input)
		{
			foreach(child; prop.children)
			{
				if(child.type == "system")
				{
					if(child.name == "Physics")
					{
					//	game.physics.serialize(child);
					}
					else if(child.name == "Network")
					{
					//	game.network.serialize(child);
					}
				}
			}
		}
		
		root.serialize(prop);
	}

	@property Entity root()
	{
		return _root;
	}

//	Camera mainCamera = nullptr;
//	string name = "Scene";
	string currentFile; 

private:
	uint GenerateEntityId()
	{
		return ++currentId;
	}
	
	@property void root(Entity ent)
	{
		_root = ent;
	}

	uint currentId = 0;
	Entity _root;
}