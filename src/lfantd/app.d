
import core.time;

import std.stdio;
import std.typecons;
import std.container;
import std.datetime;

import dcollections.ArrayList;
import dcollections.Deque;
import dcollections.LinkList;

import lfant.game;

int main()
{
/*	{
		Game game = new Game;
		game.initialize();
		for(int i = 0; i < 200; ++i)
		{
			game.update();
		}
		game.deinit();
	}*/

	class Tester
	{
		int i = 1;
		int k = 2;

		Tester opUnary(string op:"++")()
		{
			i++;
			return this;
		}
	}


	const(int) count = 300;
	TickDuration start;

	ulong diffe()
	{
		return (TickDuration.currSystemTick - start).nsecs;
	}

	alias ar_type = Tester;

	SList!ar_type _slist;
	ArrayList!ar_type _arlist = new ArrayList!ar_type;
	Deque!ar_type _array = new Deque!ar_type;
	Tester[] _carr;

	writeln("insertion:");
	start = TickDuration.currSystemTick;
	for(int i = 0; i < count; ++i) { _slist.insertFront(new ar_type()); }
	writeln("slist: ", diffe);
	
	start = TickDuration.currSystemTick;
	for(int i = 0; i < count; ++i) { _array.add(new ar_type()); }
	writeln("deque: ", diffe);

	start = TickDuration.currSystemTick;
	for(int i = 0; i < count; ++i) { _arlist.add(new ar_type()); }
	writeln("arraylist: ", diffe);
	
	start = TickDuration.currSystemTick;
	for(int i = 0; i < count; ++i) { _carr ~= new ar_type(); }
	writeln("carr: ", diffe);



	writeln("iteration:");
	start = TickDuration.currSystemTick;
	foreach(i; _slist) { i++; }
	writeln("slist: ", diffe);
	
	start = TickDuration.currSystemTick;
	foreach(i; _array) { i++; }
	writeln("deque: ", diffe);

	start = TickDuration.currSystemTick;
	foreach(i; _arlist) { i++; }
	writeln("arraylist: ", diffe);
	
	start = TickDuration.currSystemTick;
	foreach(i; _carr) { i++; }
	writeln("carr: ", diffe);

	
	writeln("about to return");
	return 1;
}