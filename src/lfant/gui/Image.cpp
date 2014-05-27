
#include "lfant/gui/Image.h"
#include <lfant/Texture.h>

namespace lfant {
namespace gui {

void Image::Create(ivec2 size, Color color)
{
	data.resize(size.x*size.y, color);
	this->size = size;
}

void Image::SetPixel(ivec2 pos, Color color)
{
	if(pos.y > size.y || pos.x > size.x)
	{
		return;
	}
	
	uint idx = (pos.y*size.x) + pos.x;
	
	data[idx] = color;
}

ivec2 Image::GetSize()
{
	return size;
}

void Image::Copy(Texture* tex)
{
//	Create(tex->GetSize(), {255,255,255,0});
	
	tex->Bind();
	glGetTexImage( tex->GetMode(), 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0] );
	tex->Unbind();
}

}
}