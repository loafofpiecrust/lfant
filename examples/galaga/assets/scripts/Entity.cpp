
class Object
{

}

class Entity : Object
{
	void Init()
	{
		TestFunc();
		TestFunc();
		TestFunc();
	}
}

void main()
{
	Entity ent;
	ent.Init();
}
