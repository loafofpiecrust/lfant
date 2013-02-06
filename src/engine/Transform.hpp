/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
 *	Created: 2012-07-26 by Taylor Snead
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
#include "Object.hpp"
#include "Property.hpp"

using namespace function_types;

namespace sfs
{
	class Entity;

	/** @addtogroup Engine
	 *	 @{
	 */
	/** @addtogroup Utilities
	 *	 @{
	 */

	class Transform : public Object
	{
	public:
		Transform() {}
		Transform(Entity* owner);
		virtual ~Transform()
		{
		}

		void Translate(vec3 pos);
		void Translate(float x, float y, float z);
		void Rotate(vec3 rot);
		void Scale(vec3 scl);

		const Entity* owner;

		Transform* parent;

		vec3 direction;
		vec3 right;
		vec3 up;

	public:

		// Properties

		vec3& GetPos();
		void SetPos(vec3 pos);
		vec3 _position;

		quat GetRotQuat();
		void SetRotQuat(quat rot);
		quat _rotationQuat;

		vec3& GetRot();
		void SetRot(vec3 rot);
		vec3 _rotation;

		vec3 GetScale();
		void SetScale(vec3 scl);
		vec3 _scale;

		vec3 GetWorldPos();
		void SetWorldPos(vec3 pos);

		quat GetWorldRotQuat();
		void SetWorldRotQuat(quat rot);

		vec3 GetWorldRot();
		void SetWorldRot(vec3 rot);

		vec3 GetWorldScale();
		void SetWorldScale(vec3 scl);

		mat4 GetMatrix();

		//Property<Transform, boost::function_types::result_type<decltype(&Transform::GetPos)>::type, mpl::at_c<parameter_types<decltype(&Transform::SetPos)>, 1>::type> posir;

		PROP_RW(Transform, position, GetPos, SetPos)
		PROP_RW(Transform, rotation, GetRot, SetRot)
		PROP_RW(Transform, rotationQuat, GetRotQuat, SetRotQuat)
		PROP_RW(Transform, scale, GetScale, SetScale)

		PROP_RW(Transform, worldPosition, GetWorldPos, SetWorldPos)
		PROP_RW(Transform, worldRotation, GetWorldRot, SetWorldRot)
		PROP_RW(Transform, worldRotationQuat, GetWorldRotQuat, SetWorldRotQuat)
		PROP_RW(Transform, worldScale, GetWorldScale, SetWorldScale)

		PROP_RO(Transform, matrix, GetMatrix)

		void SetDirection();
	};

	/** @} */
	/** @} */
}
