
// internal
#include <volymerna/File.h>

// external

namespace volymerna {

class Buffer : public File
{
public:
	Buffer();
	~Buffer();
	
	virtual void Load(string path);

	virtual void Play(float time);

protected:

private:

};

}