/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2012-10-29 by Taylor Snead
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

#include <lfant/AudioListener.h>
#include "Transform.h"
#include "Rigidbody.h"
#include <ik_ISoundEngine.h>

// External

// Internal


namespace lfant
{

IMPLEMENT_TYPE(Component, AudioListener)

void AudioListener::Init()
{
	ConnectEvent(SENDER(owner->transform, SetPosition), RECEIVER(this, OnSetPosition));
	ConnectComponent<Rigidbody>(rigidbody);
}

void AudioListener::OnSetPosition(vec3 pos)
{
	vec3 vel(0);
	if(rigidbody)
	{
		vel = rigidbody->GetVelocity();
	}

/*	GetGame()->audio->engine->setListenerPosition({0.0f, 0.0f, 0.0f},
									vec3_cast<irrklang::vec3df>(owner->transform->GetDirection()),
									vec3_cast<irrklang::vec3df>(vel),
									vec3_cast<irrklang::vec3df>(owner->transform->GetUp())
								   );*/
}


}
