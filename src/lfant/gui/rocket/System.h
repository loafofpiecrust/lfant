
#include <Rocket/Core/SystemInterface.h>

namespace lfant {
namespace gui {
namespace rocket {

class System : public Rocket::Core::SystemInterface
{
private:
	virtual float GetElapsedTime();
};

}
}
}