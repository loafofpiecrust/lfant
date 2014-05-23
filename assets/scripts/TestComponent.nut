
class Player extends Component
{
	grease = 1.0;
	points = 0;
	money = 0;

	taste = Taste();
	smell = Taste();

	inventory = null;


	constructor()
	{
		base.constructor(this);
	}

	function Load(prop)
	{
		base.Load(prop);
	}

	function Save(prop)
	{
		base.Save(prop);
		smell.Save(prop.AddChild("smell"));
		taste.Save(prop.AddChild("taste"));

		prop.Set("grease", grease);
	}

	function Init()
	{
		base.Init();

		ConnectEvent(owner, "CollideEnter", this, OnCollideEnter);
	}

	function Update()
	{
		base.Update();
	}

	function Deinit()
	{
		base.Deinit();
	}
}