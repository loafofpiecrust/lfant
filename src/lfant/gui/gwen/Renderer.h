

#include <Gwen/BaseRender.h>

namespace lfant {
namespace gui {
namespace gwen {

class Renderer : public Gwen::Renderer::Base
{
public:
	Renderer();
	~Renderer();

	virtual void Init();

	virtual void Begin();
	virtual void End();

	virtual void LoadTexture( Gwen::Texture* pTexture );
	virtual void FreeTexture( Gwen::Texture* pTexture );

	virtual void LoadFont( Gwen::Font* pFont );
	virtual void FreeFont( Gwen::Font* pFont );

	virtual Gwen::Point MeasureText( Gwen::Font* pFont, const Gwen::UnicodeString& text );

private:

};

}
}
}
