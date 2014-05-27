
#include "lfant/UserInterface.h"


#include <Rocket/Core/String.h>

namespace Rocket {
namespace Core {
	class Context;
	class ElementDocument;
}
}

namespace lfant {
namespace gui {
namespace rocket {

class Renderer;
class System;
	
class UserInterface : public lfant::UserInterface
{
public:
	UserInterface();
	virtual ~UserInterface();
	
	virtual void Serialize(Properties *prop);
	virtual void Save(Properties *prop) const;
	
	virtual void Init();
	virtual void Update();
//	virtual void Render();
	virtual void Deinit();
	
	virtual void OnKey(uint16 key, int mode, int mods);
	virtual void OnChar(char key);
	virtual void OnMouseButton(uint16 btn, int mode, int mods);
	virtual void OnMouseMove(ivec2 pos);
	virtual void OnWindowResize(int width, int height);
	
private:
	static Rocket::Core::String ConvertString(const string& str);
	
	Rocket::Core::Context* context;
	ptr<gui::rocket::Renderer> renderer;
	ptr<gui::rocket::System> system;
};

}
}
}