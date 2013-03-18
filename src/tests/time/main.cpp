
// External
#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;

typedef chrono::high_resolution_clock hclock;
typedef unsigned int uint;

uint iterations = 2500;

class Engine
{
public:
	virtual void Update()
	{
		for(uint i=0; i < iterations; ++i)
		{
			++prop;
		}
	}

	int prop = 0;
};

class Game : public Engine
{
public:
	virtual void Update()
	{
		++prop;
	}
};

Engine* engine = new Engine;
Engine* game = new Game;

int main()
{
	while(1)
	{
		cout << "How many iterations?\n";
		cin >> iterations;
		cout << "\n";

		auto start = hclock::now();
		engine->Update();
		chrono::nanoseconds length = hclock::duration(hclock::now()-start);

		cout << "Loop within function took: " << length.count() << " nanoseconds\n";
		cout << "\t\t" << duration_cast<microseconds>(length).count() << " microseconds\n";
		cout << "\t\t" << duration_cast<milliseconds>(length).count() << " milliseconds\n";

		start = hclock::now();
		for(uint i = 0; i < iterations; ++i)
		{
			game->Update();
		}
		length = hclock::duration(hclock::now()-start);

		cout << "Loop around function took: " << length.count() << " nanoseconds\n";
		cout << "\t\t" << duration_cast<microseconds>(length).count() << " microseconds\n";
		cout << "\t\t" << duration_cast<milliseconds>(length).count() << " milliseconds\n";
		cout << "\n\n";
	}
	return 0;
}

