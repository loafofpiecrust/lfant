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
#include "Transform.hpp"

// External

// Internal
#include "Entity.hpp"
#include "Math.hpp"
#include "Console.hpp"

namespace sfs
{

Transform::Transform(Entity* owner) :
	owner(owner)
{
	/*
	if (owner)
	{
		if (owner->parent)
		{
			parent = owner->parent->transform;
		}
		else
		{
			parent = nullptr;
		}
	}*/
}

vec3& Transform::GetPosition()
{
	return position;
}

void Transform::SetPosition(vec3 pos)
{
	position = pos;
	Trigger("SetPosition", position);
}

quat Transform::GetRotationQuat()
{
	return quat(rotation);
}

void Transform::SetRotationQuat(quat rot)
{
	rotationQuat = rot;
	rotation = eulerAngles(rot);
//	Trigger("SetRotation", rotation);
}

vec3& Transform::GetRotation()
{
	return rotation;
}

void Transform::SetRotation(vec3 rot)
{
	//rot.x = rollover(rot.x, 0.0f, 360.0f);
	//rot.y = rollover(rot.y, 0.0f, 360.0f);
	//rot.z = rollover(rot.z, 0.0f, 360.0f);
	//rotation = radians(rot);
	rotation = rot;
	rotationQuat = quat(rotation);
}

vec3& Transform::GetScale()
{
	return scale;
}

void Transform::SetScale(vec3 scl)
{
	scale = scl;
	Trigger("SetScale", scale);
}

vec3 Transform::GetWorldPosition()
{
	if(owner->parent)
	{
		return owner->parent->transform->GetWorldPosition() + GetPosition();
	}
	else
	{
		return GetPosition();
	}
}

void Transform::SetWorldPosition(vec3 pos)
{
	SetPosition(pos - parent->GetWorldPosition());
}

quat Transform::GetWorldRotationQuat()
{
	return quat(GetWorldRotation());
}

void Transform::SetWorldRotationQuat(quat rot)
{
	SetRotation(eulerAngles(rot) - parent->GetWorldRotation());
}

vec3 Transform::GetWorldRotation()
{
	if(owner->parent)
	{
		return owner->parent->transform->GetWorldRotation() + GetRotation();
	}
	else
	{
		return GetRotation();
	}
}

void Transform::SetWorldRotation(vec3 rot)
{
	SetRotation(rot - parent->GetWorldRotation());
}

vec3 Transform::GetWorldScale()
{
	if(owner->parent)
	{
		return owner->parent->transform->GetWorldScale() * GetScale();
	}
	else
	{
		return GetScale();
	}
}

void Transform::SetWorldScale(vec3 scl)
{
	SetScale(scl - parent->GetWorldScale());
}

mat4 Transform::GetMatrix()
{
	mat4 matrix;
	matrix = glm::translate(mat4(1.0f), GetWorldPosition());
	matrix *= glm::mat4_cast(GetWorldRotationQuat());
	matrix = glm::scale(matrix, GetWorldScale());
	return matrix;
}

void Transform::SetDirection()
{
	vec3 rot = GetWorldRotation();
	direction = vec3(
				cos(rot.x) * sin(rot.z),
				sin(rot.x),
				cos(rot.x) * cos(rot.z)
				);

	right = vec3(
				sin(rot.z - 3.14f / 2.0f),
				0,
				cos(rot.z - 3.14f / 2.0f)
				);

	up = cross(right, direction);
}

void Transform::Translate(vec3 pos)
{
	SetPosition(position + pos);
}

void Transform::Rotate(vec3 rot)
{
	SetRotation(rotation + rot);
	//rotationQuat = quat(rotation);
	//_rotationQuat = rotate( _rotationQuat, rot.x, xdir );
	//_rotationQuat = rotate( _rotationQuat, rot.y, ydir );
	//_rotationQuat = rotate( _rotationQuat, rot.z, zdir );
//	Trigger("SetRotation", rotation);
}

void Transform::Scale(vec3 scl)
{
	SetScale(scale * scl);
//	Trigger("SetScale", scale);
}

}
