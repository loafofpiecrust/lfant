
// internal
#include <volymerna/File.h>

// external


class PaStream;

namespace volymerna {

class Stream : public File
{
public:
	Stream();
	~Stream();

	virtual void Load(string path);

	virtual void Play(float time);

protected:

private:
	PaStream* rawStream = nullptr;
};

}