module lfant.input;

import std.container;
import std.stdio;

import gl3n.linalg;
import gl3n.math;

import lfant.game;
import lfant.properties;

class Axis
{
	this(string name)
	{
		this.name = name;
	}
	
	string name;
	ushort positive;
	ushort negative;
	float sensitivity;
	float dead;
	bool snap;
	/// @todo Joystick axis

	/// Number of the controller to use. 0 means all.
	byte joyNum = 0;
	
	@property
	{
		float value() const
		{
			return _value;
		}
		private ref float value()
		{
			return _value;
		}
	}

private:
	byte held = 0;
	bool down = false;
	bool up = false;
	
	float _value = 0.0f;
}
	
class Input : Subsystem
{
	this(Game g)
	{
		super(g);
	}

	~this()
	{
		writeln("input");
	}
	
	override void initialize()
	{
		loadFile("settings/input.prop");
	}
	
	override void update(in float delta)
	{
		foreach(axis; axes)
		{
			if(axis.down || axis.up)
			{
				axis.down = false;
				axis.up = false;
			}
			if(axis.snap || (abs(axis.value) <= axis.dead && axis.held == 0))
			{
				continue;
			}
	
			if(axis.held == 1)
			{
				axis.value += axis.sensitivity * delta;
			}
			else if(axis.held == -1)
			{
				axis.value += axis.sensitivity * delta;
			}
			else
			{
				if(axis.value > axis.dead)
				{
					axis.value -= axis.sensitivity * delta * 1.75;
				}
				else if(axis.value < -axis.dead)
				{
					axis.value += axis.sensitivity * delta * 1.75;
				}
			}
	
			if(abs(axis.value) > 1.0f)
			{
				axis.value = clamp(axis.value, -1.0f, 1.0f);
			}
		}
	}
	
	override void serialize(Properties prop)
	{
		auto addAxisFunc = function void(Axis axis, Properties b)
		{
			if(b.mode == Properties.Mode.Input)
			{
				string pos, neg;
				b.value("positive", pos);
				b.value("negative", neg);
			//	axis.positive = Key[pos];
			//	axis.negative = Key[neg];
			}
			else
			{
			//	b->SetString("positive", Key[axis.positive]);
			//	b->SetString("negative", Key[axis.negative]);
			}
			b.value("sensitivity", axis.sensitivity);
			b.value("dead", axis.dead);
			b.value("snap", axis.snap);
			b.value("joyNum", axis.joyNum);
		};
	
		if(prop.mode == Properties.Mode.Input)
		{
			writeln("\ninput mode");
			foreach(b; prop.children)
			{
			//	if(!b->IsType("axis") || b->name.empty()) continue;
	
				Axis axis = new Axis(b.name);
				writeln("adding axis ", b.name, " yowza");
				addAxisFunc(axis, b);
				axes.insert(axis);
			}
		}
		else
		{
			foreach(axis; axes)
			{
				addAxisFunc(axis, prop.child("axis", axis.name));
			}
		}
	}
	
	void addAxis(Axis a)
	{
		axes.insert(a);
	}
	
	Axis getAxis(in string name)
	{
		foreach(ref axis; axes)
		{
			if(axis.name == name)
			{
				return axis;
			}
		}
		return null;
	}
	
	byte getButtonDown(in string name)
	{
		auto axis = getAxis(name);
		if(axis)
		{
			return axis.down * axis.held;
		}
		else
		{
			return 0;
		}
	}

	void onKey(int key, int action, int mods)
	{
		triggerEvent("KeyPress", cast(ushort)key, action);
		triggerEvent("KeyPress", cast(ushort)key, action, mods);
		foreach(axis; axes)
		{
			if(key == axis.positive)
			{
				axis.held = cast(byte)action;
				if(action == 1)
				{
					axis.down = true;
					if(axis.snap)
					{
						//axis.value = axis.negHeld ? 0.0f : 1.0f;
						axis.value += 1.0f;
					}
					triggerEvent(axis.name~"_Down");
				}
				else if(action == 0)
				{
					axis.up = true;
					if(axis.snap)
					{
						//axis.value = axis.negHeld ? -1.0f : 0.0f;
						axis.value -= 1.0f;
					}
					triggerEvent(axis.name~"_Up");
				}
				triggerEvent(axis.name, axis.value);
			}
			else if(key == axis.negative)
			{
				axis.held = cast(byte)-action;
				if(action == 1)
				{
					if(axis.snap)
					{
						//axis.value = axis.posHeld ? 0.0f : -1.0f;
						axis.value -= 1.0f;
					}
				}
				else if(action == 0)
				{
					if(axis.snap)
					{
						//axis.value = axis.posHeld ? 1.0f : 0.0f;
						axis.value += 1.0f;
					}
				}
				triggerEvent(axis.name, axis.value);
			}
		}
	}
	void onChar(uint key) nothrow
	{
		
	}
	void onMouseMove(vec2i pos) nothrow
	{
		
	}
	void onMouseButton(int btn, int action, int mods) nothrow
	{
		
	}
	
private:
	Array!Axis axes;
}