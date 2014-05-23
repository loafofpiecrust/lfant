
#include <lfant/stdafx.h>
//#include <SFML/Graphics/Color.hpp>

namespace lfant {

//using sf::Color;
	
class Color : public glm::u8vec4
{
public:
	Color(value_type r, value_type g, value_type b) :
		glm::u8vec4(r,g,b,255)
	{
	}
	
	Color(value_type r, value_type g, value_type b, value_type a) :
		glm::u8vec4(r,g,b,a)
	{
	}
};

}