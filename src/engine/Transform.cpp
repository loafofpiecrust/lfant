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
#include "MathUtil.hpp"
#include "Console.hpp"

using namespace sfs;

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

vec3& Transform::GetPos()
{
	return _position;
}

void Transform::SetPos(vec3 pos)
{
	_position = pos;
	trigger("SetPos", position);
}

quat Transform::GetRotQuat()
{
	return quat(rotation * degToRad);
}

void Transform::SetRotQuat(quat rot)
{
	_rotationQuat = rot;
	rotation = eulerAngles(rot);
	trigger("SetRot", rotation);
}

vec3& Transform::GetRot()
{
	Log("Transform::GetRot(): Getting rot");
	return _rotation;
}

void Transform::SetRot(vec3 rot)
{
	_rotation = rot;
	_rotationQuat = quat(_rotation * degToRad);
}

vec3 Transform::GetScale()
{
	return _scale;
}

void Transform::SetScale(vec3 scl)
{
	_scale = scl;
	trigger("SetScale", scale);
}

vec3 Transform::GetWorldPos()
{
	if(owner->parent)
	{
		return owner->parent->transform->worldPosition + position;
	}
	else
	{
		return position;
	}
}

void Transform::SetWorldPos(vec3 pos)
{
	position = pos - parent->worldPosition;
}

quat Transform::GetWorldRotQuat()
{
	return quat(worldRotation * degToRad);
}

void Transform::SetWorldRotQuat(quat rot)
{
	rotation = eulerAngles(rot) - parent->worldRotation();
}

vec3 Transform::GetWorldRot()
{
	if(owner->parent)
	{
		return owner->parent->transform->GetWorldRot() + _rotation;
	}
	else
	{
		return _rotation;
	}
}

void Transform::SetWorldRot(vec3 rot)
{
	rotation = rot - parent->worldRotation;
}

vec3 Transform::GetWorldScale()
{
	return parent->worldScale * scale;
}

void Transform::SetWorldScale(vec3 scl)
{
	scale = scl - parent->worldScale;
}

mat4 Transform::GetMatrix()
{
	mat4 matrix;
	matrix = glm::translate(mat4(1.0f), worldPosition());
	matrix *= glm::mat4_cast(worldRotationQuat());
	matrix = glm::scale(matrix, worldScale());
	return matrix;
}

void Transform::SetDirection()
{
	Log("Transform::SetDirection(): Entered function");
	vec3 rot = GetWorldRot();
	Log("Transform::SetDirection(): Grabbed world rot");
	rot *= vec3(degToRad);
	Log("Transform::SetDirection(): Transmute worldRotation to radians");
	direction = vec3(cos(rot.x) * sin(rot.z), sin(rot.x), cos(rot.x) * cos(rot.z));
	Log("Transform::SetDirection(): Set direction");
	right = vec3(sin(rot.x - pi / 2.00f), 0, cos(rot.x - pi / 2.00f));
	Log("Transform::SetDirection(): Set right");
	up = cross(right, direction);
	Log("Transform::SetDirection(): Set up");
}

void Transform::Translate(vec3 pos)
{
	position += pos;
}

void Transform::Translate(float x, float y, float z)
{
	Translate(vec3(x, y, z));
}

void Transform::Rotate(vec3 rot)
{
	rotation += rot;
	//rotationQuat = quat(rotation);
	//_rotationQuat = rotate( _rotationQuat, rot.x, xdir );
	//_rotationQuat = rotate( _rotationQuat, rot.y, ydir );
	//_rotationQuat = rotate( _rotationQuat, rot.z, zdir );
	trigger("SetRot", rotation);
}

void Transform::Scale(vec3 scl)
{
	scale *= scl;
	trigger("SetScale", scale);
}
