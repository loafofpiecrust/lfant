/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-10-01 by Taylor Snead
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

// External

// Internal
#include <lfant/Renderable.h>
#include <lfant/Shader.h>

namespace lfant
{
/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Rendering
 *	 @{
 */

/**
 *
 */
class Light : public Renderable
{
public:
	Light();
	~Light();

	void Save(Properties* prop);
	void Load(Properties* prop);

	void Init();
	void Update();

	virtual void BeginRender() {};
	virtual void Render();
	virtual void EndRender() {};

protected:
	vec3 color = vec3(1);
	ptr<Shader> shader = new Shader;

private:

};

/** @} */
/** @} */
}
