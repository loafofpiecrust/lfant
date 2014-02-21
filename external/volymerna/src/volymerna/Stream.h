
// internal
#include <volymerna/File.h>

// external
#include <fstream>


namespace volymerna {

class Stream : public File
{
public:
	Stream(std::string path, Source* src = nullptr);
	~Stream();

//	virtual void Load(std::string path);

	virtual void PlayChunk();

	virtual void Seek(float time);

protected:

private:
	std::fstream file;
};

}
