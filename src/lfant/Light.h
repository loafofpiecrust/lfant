/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
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


	virtual void Serialize(Properties* prop);

	virtual void Init();
	virtual void Update();

	virtual void Render();
	virtual void RenderStencil() {};

protected:
	std::shared_ptr<Shader> shader = nullptr;
	vec3 color = vec3(1);

private:

};

/** @} */
/** @} */
}
