
// internal

// external

namespace volymerna {

class File
{
public:
	File();
	~File();

	virtual void Load(string path) = 0;
	virtual void Play(float time, float volume) = 0;
	virtual void GetVolume(float time) = 0;

protected:

	float totalTime = 1.0f;

private:
};

}