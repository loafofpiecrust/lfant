/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
 *	Created: 2013-02-13 by Taylor Snead
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *	http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 *
 ******************************************************************************/

// External
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/Renderer.h>
#include <CEGUI/SchemeManager.h>
#include <CEGUI/DefaultResourceProvider.h>

// Internal
#include "UserInterface.hpp"
#include "Thread.hpp"
#include "Engine.hpp"
#include "FileManager.hpp"
#include "Console.hpp"

namespace sfs
{

UserInterface::UserInterface()
{
}

UserInterface::~UserInterface()
{
}

void UserInterface::Init()
{
	/*
	Log("UserInterface::Init: Begin");
	CEGUI::Logger::getSingletonPtr();
	Log("Got log single");
	renderer = &CEGUI::OpenGLRenderer::bootstrapSystem();
	Log("UserInterface::Init: Spawned renderer");

	CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider());
	rp->setResourceGroupDirectory("imagesets", game->fileManager->GetGameFile("gui/imagesets").string());
	Log("UserInterface::Init: Set a resource dir");
	rp->setResourceGroupDirectory("schemes", game->fileManager->GetGameFile("gui/schemes").string());
	rp->setResourceGroupDirectory("styles", game->fileManager->GetGameFile("gui/styles").string());
	rp->setResourceGroupDirectory("fonts", game->fileManager->GetGameFile("gui/fonts").string());
	rp->setResourceGroupDirectory("looknfeel", game->fileManager->GetGameFile("gui/looks").string());

	CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
	Log("UserInterface::Init: Created scheme!");
	CEGUI::System::getSingleton().setDefaultFont( "DejaVuSans-10" );
	Log("UserInterface::Init: Set default font");
	CEGUI::System::getSingleton().setDefaultMouseCursor( "TaharezLook", "MouseArrow" );
	Log("UserInterface::Init: Set default cursor");
	rootWindow = CEGUI::WindowManager::getSingleton().createWindow( "DefaultWindow", "_MasterRoot" );
	Log("UserInterface::Init: Created root window");
	CEGUI::System::getSingleton().setGUISheet( rootWindow );
	Log("UserInterface::Init: Set root window as master parent");
	CEGUI::Window *imageWindow = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage", "PrettyWindow" );
	Log("UserInterface::Init: Created static window");
	*/
}

void UserInterface::Update()
{
//	CEGUI::System::getSingleton().renderGUI();
}

void UserInterface::OnDestroy()
{
}

}
