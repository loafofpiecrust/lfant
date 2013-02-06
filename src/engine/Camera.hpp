/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
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
#include "stdafx.hpp"

// External

// Internal
#include "Component.hpp"
#include "Range.hpp"
#include "Rect.hpp"

namespace sfs
{
	/** @addtogroup Engine
	 *	@{
	 */
	/** @addtogroup Components
	 *	@{
	 */

	/**	This class handles a Camera Component for the Owner Entity.
	 *	@details
	 *		This is a Component to allow easy use of Cameras for Players,
	 *		and the ability to add/remove a Camera without having to create
	 *		and destroy the Entity. It could be an Entity, and allowed to be
	 *		a child of the one in need of it, but then I wouldn't be taking
	 *		advantage of having a Component-based Entity system.
	 *	@todo
	 */
	class Camera : public Component
	{
		friend class Renderer;

	public:
		Camera();
		virtual ~Camera();

		virtual void Init();
		virtual void Update();
		//virtual void OnDestroy();

		/**	Sets the projection of this Camera for viewing.
		 *	@param fov The FoV to use, eg. 70
		 *	@param aspect Aspect ratio to use, eg. 16/10 or 16/9
		 *	@param min The closest distance to render.
		 *	@param max The farthest distance to render.
		 */
		void SetProjection(float fov, float aspect, float min, float max);

		/**	Updates the projection to use the current settings
		 */
		void UpdateProjection();

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
		float fov = 100.0f;

		/// Aspect ratio, eg. 16.0f/9.0f
		float aspectRatio = 16 / 9;

		/// The projection matrix for rendering the projection.
		mat4 projection = mat4(0);

		vec3 direction;
		vec3 right;
		vec3 up;
		mat4 view = mat4(0);

		/// The viewing range (near and far) of this camera's view, eg. (0.1f, 1000.0f)
		Range<float> viewRange = { 0.1f, 1000.0f };

		vec3 lastPos;

		Rect<uint32_t> _viewport;

		enum ProjectionMode
		{
			PM_PERSPECTIVE, PM_ORTHO
		};
		ProjectionMode projectionMode = PM_PERSPECTIVE;

//		PROP_RW(Camera, viewport, GetViewport, SetViewport)

	protected:

	private:

	};

	/** @} */
	/** @} */
}
