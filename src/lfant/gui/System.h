
// External
#include <Rocket/Core/SystemInterface.h>

// Internal

namespace lfant
{
namespace gui
{

class System : public Rocket::Core::SystemInterface
{
public:
	System();
	virtual ~System();

protected:
	virtual float GetElapsedTime();
private:

};

}
}
