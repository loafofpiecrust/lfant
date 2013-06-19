/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2013-06-18 by Taylor Snead
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
#pragma once

// internal
#include <lfant/Component.h>

// External

namespace lfant {
namespace galaga {

class FollowEntity : public lfant::Component
{
	DECLARE_COMP(FollowEntity)
public:
	FollowEntity();
	~FollowEntity();

	void Load(Properties* prop);
	void Save(Properties* prop);

	void Init();
	void Update();

	bool useX = true;
	bool useY = true;
	bool useZ = true;
	float speed = 3.5f;
	float damping = 1.0f;
	Entity* entity = nullptr;
protected:

private:
};

}
}