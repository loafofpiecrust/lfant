/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-10-28 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License at
*
*		http://www.apache.org/licenses/LICENSE-2.0
*
*	Unless required by applicable law or agreed to in writing, software
*	distributed under the License is distributed on an "AS IS" BASIS,
*	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*	See the License for the specific language governing permissions and
*	limitations under the License.
*
******************************************************************************/

#include <lfant/Object.h>

// External

// Internal

namespace lfant
{

Object::Object()
{
}

Object::~Object()
{
}

void Object::Init()
{
	Bind();
}

void Object::Destroy()
{
	OnDestroy();
}

void Object::Destroy(Object* obj)
{
	for(auto& evt : events)
	{
		delete evt;
	}
	obj->Destroy();
}

void Object::Bind()
{
	//SClass<Object> cls;
	/*cls.Func("connect", &Object::connect);
	   cls.Func("trigger", &Object::trigger);

	   cls.Func("Init", &Object::Init);
	   cls.Func("Update", &Object::Update);
	   cls.Func("Destroy", &Object::Destroy);

	   cls.Func("OnDestroy", &Object::OnDestroy);*/
	//cls.FUNC( Object, Init );
	//cls.FUNC( Object, Update );
}

void Object::LoadFile(string path)
{
	ptr<Properties> prop {new Properties};
	prop->LoadFile(path);
	string type = RemoveScoping(Type(this));
	to_lower(type);
	if(Properties* pc = prop->GetChild(type))
	{
		Load(pc);
	}
}

void Object::Load(Properties *prop)
{
}

void Object::SaveFile(string path)
{
	ptr<Properties> prop {new Properties};
	Save(prop);
	prop->SaveFile(path);
}

void Object::Save(Properties *prop)
{
	string type = RemoveScoping(Type(this));
	to_lower(type);
	prop->type = type;
}

}
