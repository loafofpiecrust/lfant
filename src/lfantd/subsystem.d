
module lfant.subsystem;

import lfant.game;
import lfant.gameobject;

class Subsystem : GameObject
{
public:
	this()
	{
	}

	this(Game g)
	{
		this._game = g;
	}
	
	override void deinit()
	{
	//	_game = null;
	}
	
	@property
	{
		override Game game()
		{
			return _game;
		}
		protected void game(Game g)
		{
			_game = g;
		}
	}
	
private:
	Game _game;
}