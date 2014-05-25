/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2012-07-19 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/

#include <lfant/Network.h>

// Internal
#include <lfant/Console.h>
#include <lfant/Thread.h>

#include <lfant/net/User.h>

// External

namespace lfant
{

Network::Network(Game* game) :
	Subsystem(game)
{
	//Init();
}

Network::~Network()
{
}

void Network::Init()
{
	GetGame()->Log("Network::Init: Touch.");

	Subsystem::Init();
}

void Network::Update()
{
	//std::cout << "net update\n";
	for(auto& usr : users)
	{
		usr->Update();
	}
}

void Network::RunLoop()
{
	while(!GetGame()->IsExited())
	{
	//	mutex.lock();
		Update();
	//	mutex.unlock();
	}
}

void Network::Save(Properties *prop) const
{
//	Properties* arr = prop->AddChild("users");
	for(auto& usr : users)
	{
		usr->Serialize(prop->Child("user", ""));
	//	usr->Save(new ValueProperties(prop, "User"));
	}
}

void Network::Serialize(Properties *prop)
{
//	deque<Properties*> cons = prop->GetChildren("user");
//	for(auto& pcon : cons)
	{
	//	net::Connection* con = new net::Connection;
	//	con->Load(pcon);
	//	connections.push_front(con);
	}
}

net::User* Network::GetUser(string name) const
{
	for(auto& usr : users)
	{
		if(usr->name == name)
		{
			return usr;
		}
	}
	return nullptr;
}

net::User* Network::AddUser(string type)
{
	return net::User::typeRegistry.New(type);
}

}

