
#include "lfant/gui/gwen/Renderer.h"

namespace lfant {
namespace gui {
namespace gwen {

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

void Renderer::Init()
{
	Gwen::Renderer::Base::Init();
}

void Renderer::Begin()
{
	Gwen::Renderer::Base::Begin();
}

void Renderer::End()
{
	Gwen::Renderer::Base::End();
}

void Renderer::LoadTexture(Gwen::Texture* pTexture)
{
	Gwen::Renderer::Base::LoadTexture(pTexture);
}

void Renderer::FreeTexture(Gwen::Texture* pTexture)
{
	Gwen::Renderer::Base::FreeTexture(pTexture);
}


void Renderer::LoadFont(Gwen::Font* pFont)
{
	Gwen::Renderer::Base::LoadFont(pFont);
}

void Renderer::FreeFont(Gwen::Font* pFont)
{
	Gwen::Renderer::Base::FreeFont(pFont);
}


Gwen::Point Renderer::MeasureText(Gwen::Font* pFont, const Gwen::UnicodeString& text)
{
	return Gwen::Renderer::Base::MeasureText(pFont, text);
}


}
}
}
