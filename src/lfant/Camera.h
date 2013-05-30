#ifndef HEADER_62C981C59A7F3A65
#define HEADER_62C981C59A7F3A65

/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-07-23 by Taylor Snead
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

#include <lfant/stdafx.h>

// External

// Internal

#include <lfant/Component.h>
#include <lfant/Range.h>
#include <lfant/Rect.h>

namespace lfant
{
/** @addtogroup Game
 *	@{
 */
/** @addtogroup Rendering
 *	@{
 */

/**	This class handles a Camera Component for the Owner Entity.
 *		This is a Component to allow easy use of Cameras for Players,
 *		and the ability to add/remove a Camera without having to create
 *		and destroy the Entity. It could be an Entity, and allowed to be
 *		a child of the one in need of it, but then I wouldn't be taking
 *		advantage of having a Component-based Entity system.
 */
class Camera : public Component
{
	DECLARE_COMP(Camera)
	friend class Renderer;
public:
	Camera();
	virtual ~Camera();

	virtual void Init();
	virtual void Update();
	virtual void OnDestroy();

	/**
	 *	Updates the projection to use the current settings
	 */
	void UpdateProjection();

	mat4 GetProjection();
	mat4 GetView();

	void UpdateView();
	void OnViewChange(vec3 change)
	{
		UpdateView();
	}

	/**	Sets the Field Of View for this Camera's perspective.
	 */
	void SetFOV(float fov);

	/**	Sets the Aspect Ratio to use.
	 */
	void SetRatio(float ratio);

	/**	Sets the viewing range to use (near and far).
	 */
	void SetRange(float near, float far);

	/// Field of view, eg. 40.0f
	float fov = 45.0f;

	/// Aspect ratio, eg. 16.0f/9.0f
	float aspectRatio = 16 / 9;

	/// The viewing range (near and far) of this camera's view, eg. (0.1f, 1000.0f)
	Range<float> viewRange = { 0.1f, 1000.0f };

	vec3 lastPos;

	Rect<uint32_t> _viewport;

	enum class Mode : byte
	{
		Perspective,
		Orthographic
	};
	Mode mode = Mode::Perspective;

//		PROP_RW(Camera, viewport, GetViewport, SetViewport)

protected:

	/// Matrix for specific projection.
	mat4 projection = mat4(1);

	/// Matrix for transformations.
	mat4 view = mat4(1);

private:

};

/** @} */
/** @} */
}

#endif // header guard
