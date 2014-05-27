
#include "lfant/UserInterface.h"

#include <SFGUI/SFGUI.hpp>

namespace lfant {
namespace gui {
namespace sfgui {
	
class UserInterface : public lfant::UserInterface
{
public:
	UserInterface();
	virtual ~UserInterface();
	
	virtual void Serialize(Properties* prop);
	
	virtual void Init();
	virtual void Update();
	virtual void Render();
	
	virtual void OnKey(uint16 key, int mode, int mods) {};
	virtual void OnChar(char key) {};
	virtual void OnMouseButton(uint16 btn, int mode, int mods) {};
	virtual void OnMouseMove(ivec2 pos) {};
	virtual void OnWindowResize(int width, int height) {};
	
	void OnEvent(sf::Event evt);
	
private:
	sfg::SFGUI gui;
	
	sfg::Desktop desktop;
};

}
}
}