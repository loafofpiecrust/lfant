
// internal
#include <volymerna/System.h>

// external

namespace volymerna {

class WindowsSystem : public System
{
public:
	WindowsSystem();
	~WindowsSystem();
protected:

private:
	IXAudio2* engine = nullptr;
	IXAudio2MasteringVoice* master = nullptr;
};

}