
module lfant.gameobject;

import std.signals;
import std.path;
import std.file;

import lfant.properties;
import lfant.game;

public
{
	import std.algorithm;
	import std.array;
}

class GameObject
{
	class EventBase
	{
	public:
		void Clear()
		{
		}
	}

	class Event(T...) : EventBase
	{
		mixin Signal!(T);
	}

	class EventVar(T) : EventBase
	{
		this(T* var)
		{
			this.var = var;
		}

		T* var;
	}

	void serialize(Properties prop)
	{
	}

	void loadFile(string path)
	{
		if(path.empty) return;

		string rp = game.getAssetPath(path);
	//	string rp = path;
		if(!exists(rp)) return;

		Properties prop = new Properties();
		prop.loadFile(rp);
		prop.mode = Properties.Mode.Input;

	//	string type = type::Descope(type::Name(this));
	//	to_lower(type);
		if(Properties pc = prop.getChild(0))
		{
			serialize(pc);
		}
	}

	void saveFile(string path)
	{
		Properties prop = new Properties;
		prop.mode = Properties.Mode.Output;
		this.serialize(prop);
		prop.saveFile(path);
	}

	string typeName()()
	{
		return "";
	}

	string typeName(P1)()
	{
		return typeid(P1).toString();
	}

	string typeName(P1, P2, P...)()
	{
		return typeName!(P1)~","~typeName!(P2, P);
	}

	static void connectEvent(P...)(GameObject sender, string name, void delegate() func)
	{
		alias event_type = Event!(P);

		name ~= "("~typeName!(P)~")";
		auto evt = &sender.events[name];
		event_type evtp = null;
		if(evt && typeid(*evt) == typeid(event_type))
		{
			evtp = cast(event_type)(evt);
			evtp.connect(func);
		}
		else
		{
			evtp = new event_type();
			*evt = evtp;
		}

		if(evtp)
		{
			evtp.connect(func);
		}
	}

	void triggerEvent(P...)(string name, P args)
	{
		alias Event!(P) event_type;

		name ~= "("~typeName!(P)~")";
		auto evt = (name in events);
		if(evt && typeid(*evt) == typeid(event_type))
		{
			auto casted = cast(event_type)(evt);
			casted.emit(args);
		}
	}

	void initialize()
	{
	}

	void update(in float delta)
	{
	}
	
	void render()
	{
	}
	
	void deinit()
	{
	}

	@property abstract Game game() nothrow;

private:
	EventBase[string] events;
}