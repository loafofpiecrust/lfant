module lfant.time;

import std.datetime;
import std.array;
import std.stdio;

import lfant.subsystem;
import lfant.game;

struct Timer
{
	string name;
	float time;
}

class Time : Subsystem
{
	this(Game g)
	{
		super(g);
	}

	~this()
	{
		writeln("time");
	}
	
	override void initialize()
	{
		resetTime();
		updateTimes();
	}
	
	override void update(in float delta = 0.0f)
	{
		updateTimes();
	}
	
	void resetTime()
	{
	}
	
	void updateTimes()
	{
		auto currFrame = getTime();
		deltaTimeFixed = currFrame - lastFrame;
		lastFrame = currFrame;
		deltaTime = deltaTimeFixed * timeScale;
		frameRate = 1 / deltaTimeFixed;
	}
	
	float* getTimer(string name)
	{
		foreach(ref t; timers)
		{
			if(t.name == name)
			{
				return &t.time;
			}
		}
		return null;
	}
	
	void setTimer(Timer t)
	{
		timers ~= t;
	}
	
	
	float deltaTime = 0.0166f;
	float deltaTimeFixed = 0.0166f;
	float frameRate = 60.0f;
	float timeScale = 1.0f;
	
private:
	double getTime() const
	{
		return (cast(double)TickDuration.currSystemTick.usecs) * 0.000001;
	}

	double lastFrame;
	Timer[] timers;
}