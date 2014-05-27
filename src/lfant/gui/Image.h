
#pragma once

#include <lfant/Color.h>
#include <vector>

namespace lfant {
class Texture;

namespace gui
{

class Image
{
	friend class lfant::Texture;
public:
	
	void Create(ivec2 size, Color color);
	
	void SetPixel(ivec2 pos, Color color);
	
	ivec2 GetSize();
	
	void Copy(Texture* tex);
	
private:
	ivec2 size;
	std::vector<Color> data;
};

}
}