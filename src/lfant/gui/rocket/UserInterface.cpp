
#include "lfant/gui/rocket/UserInterface.h"

#include "lfant/gui/rocket/Renderer.h"
#include "lfant/gui/rocket/System.h"

#include "lfant/Renderer.h"
#include "lfant/Game.h"
#include "lfant/FileSystem.h"
#include "lfant/Console.h"
#include "lfant/Window.h"

#include "Rocket/Core.h"

namespace lfant {
namespace gui {
namespace rocket {

rocket::UserInterface::UserInterface()
{
}

rocket::UserInterface::~UserInterface()
{

}


Rocket::Core::String UserInterface::ConvertString(const string& str)
{
	Rocket::Core::String result;
	for(auto& c : str)
	{
		result.Append(c);
	}
	return result;
}

void UserInterface::Serialize(Properties* prop)
{
	lfant::Subsystem::Serialize(prop);
	/// @fixme
/*	std::deque<string> files;

	prop->Value("fonts", files);
	for(auto& f : files)
	{
		Rocket::Core::FontDatabase::SerializeFontFace(ConvertString(game->GetAssetPath(f).string()));
	}*/
//	Rocket::Core::FontDatabase::SerializeFontFace(ConvertString(game->GetAssetPath("gui/fonts/Ubuntu-M.ttf").string()), "Ubuntu", Rocket::Core::Font::STYLE_NORMAL, Rocket::Core::Font::WEIGHT_NORMAL);
//	Rocket::Core::FontDatabase::Initialise();

/*	prop->Value("files", files);
	for(auto& f : files)
	{
		Rocket::Core::ElementDocument* doc = context->LoadDocument(ConvertString(game->GetAssetPath(f).string()));
		doc->Show();
	}*/
}

void UserInterface::Save(Properties* prop) const
{
	lfant::Subsystem::Save(prop);
}

void UserInterface::Init()
{
	system = new rocket::System();
	renderer = new rocket::Renderer();

	Rocket::Core::SetSystemInterface(system);
	Rocket::Core::SetRenderInterface(renderer);

	ivec2 res = game->window->GetSize();
	renderer->SetViewport(res.x, res.y);

	Rocket::Core::Initialise();

	context = Rocket::Core::CreateContext("default", vec2_cast<Rocket::Core::Vector2i>(game->window->GetSize()));

	lfant::UserInterface::Init();
}

void UserInterface::Update()
{
	lfant::UserInterface::Update();

	context->Update();
	context->Render();
}

void UserInterface::Deinit()
{
	lfant::UserInterface::Deinit();
	context->UnloadAllDocuments();
	context->RemoveReference();
}

void UserInterface::OnKey(uint16 key, int mode, int mods)
{
	if(mode == 1)
	{
		context->ProcessKeyDown((Rocket::Core::Input::KeyIdentifier)key, mods);
	}
	else
	{
		context->ProcessKeyUp((Rocket::Core::Input::KeyIdentifier)key, mods);
	}
}

void UserInterface::OnChar(char key)
{
	context->ProcessTextInput(key);
}

void UserInterface::OnMouseButton(uint16 btn, int mode, int mods)
{
	if(mode == 1)
	{
		context->ProcessMouseButtonDown(btn, mods);
	}
	else
	{
		context->ProcessMouseButtonUp(btn, mods);
	}
}

void UserInterface::OnMouseMove(ivec2 pos)
{
	context->ProcessMouseMove(pos.x, pos.y, 0);
}

void UserInterface::OnWindowResize(int width, int height)
{
	renderer->SetViewport(width, height);
}


}
}
}
