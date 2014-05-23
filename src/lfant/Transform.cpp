/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2012-07-26 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
#include <lfant/Transform.h>

// External
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Internal

#include <lfant/Entity.h>
#include <lfant/util/Math.h>

#include <lfant/Console.h>
#include "ScriptSystem.h"

namespace lfant
{

Transform::Transform()
{
}

Transform::~Transform()
{

}

void Transform::Init()
{
	Component::Init();

	SetMatrix();

//	if(owner->GetParent())
	{
//		ConnectEvent(SENDER(owner->GetParent()->transform, SetPosition), RECEIVER(this, OnSetWorldPos));
//		ConnectEvent(SENDER(owner->GetParent()->transform, SetRotation), RECEIVER(this, OnSetWorldPos));
//		ConnectEvent(SENDER(owner->GetParent()->transform, SetScale), RECEIVER(this, OnSetWorldPos));
	}
}

void Transform::Save(Properties *prop) const
{
	Component::Save(prop);

	GetGame()->Log("Saving transform attr");

	prop->Set("position", position);
	prop->Set("rotation", rotation);
	prop->Set("scale", scale);
}

void Transform::Load(Properties *prop)
{
//	GetGame()->Log("Loading transform from type '"+prop->type+"', id '"+prop->id+"'.");
	Component::Load(prop);

	prop->Get("position", position);
	prop->Get("rotation", rotation);
	prop->Get("scale", scale);

	// Do this to ensure the callbacks are called
	SetPosition(position);
	SetRotation(rotation);
	SetScale(scale);

	SetMatrix();

	GetGame()->Log("Loaded position: "+lexical_cast<string>(position));
}

void Transform::ScriptBind()
{
	{
		Script::BaseClass<vec3> inst;
		inst.Var("x", &vec3::x);
		inst.Var("y", &vec3::y);
		inst.Var("z", &vec3::z);
		inst.Bind("vec3");
	}

	{
		Script::BaseClass<quat> inst;
		inst.Bind("quat");
	}

	{
		Script::Class<Transform, Component> inst;
		inst.Func("Translate", &Transform::Translate);
		inst.Bind("Transform");
	}
}

void Transform::OnSetWorldPos()
{
	TriggerEvent("SetPosition");

//	updateMatrix = true;
}

vec3 Transform::GetPosition()
{
	return position;
}

void Transform::SetPosition(vec3 pos)
{
//	pos.y = -pos.y;
	position = pos;
	TriggerEvent("SetPosition", position);
}

quat Transform::GetRotationQuat()
{
//	vec3 rot = vec3(rotation.x, rotation.y, -(rotation.z));
//	return quat(rot); // radians
//	return quat(radians(rot)); // degrees
	return rotationQuat;
}

void Transform::SetRotationQuat(quat rot)
{
//	SetRotation(eulerAngles(rot)); // radians
//	SetRotation(degrees(eulerAngles(rot))); // degrees
	rotationQuat = rot;
	rotation = degrees(eulerAngles(rot));
	TriggerEvent("SetRotation", rot);
}

vec3 Transform::GetRotation()
{
	return rotation;
}

void Transform::SetRotation(vec3 rot)
{
//	float cap = radians(360.0f); // radians
	float cap = 360.0f; // degrees
	rollover(rot.x, 0.0f, cap);
	rollover(rot.y, 0.0f, cap);
	rollover(rot.z, 0.0f, cap);

	rotation = rot;
	rotationQuat = quat(radians(rotation));
//	GetGame()->Log("Setting rotation to ", lexical_cast<string>(rotation));

	TriggerEvent("SetRotation", rot);
	TriggerEvent("SetRotation", rotationQuat);
//	TriggerEvent("SetRotation", rotationQuat);
}

vec3 Transform::GetScale()
{
	return scale;
}

void Transform::SetScale(vec3 scl)
{
	scale = scl;
	TriggerEvent("SetScale", scale);
}

vec3 Transform::GetWorldPosition()
{
	return matrix[3].xyz();
	/*
	if(owner->GetParent())
	{
		return owner->GetParent()->transform->GetWorldPosition() + GetPosition();
	}
	else
	{
		return GetPosition();
	}
	*/
}

void Transform::SetWorldPosition(vec3 pos)
{
	if(owner->GetParent())
	{
		SetPosition(pos - owner->GetParent()->transform->GetWorldPosition());
	}
	else
	{
		SetPosition(pos);
	}
}

quat Transform::GetWorldRotationQuat()
{
	if(owner->GetParent())
	{
		return owner->GetParent()->transform->GetWorldRotationQuat() * GetRotationQuat();
	//	return GetRotationQuat() * owner->parent->transform->GetWorldRotationQuat();
	}
	else
	{
		return GetRotationQuat();
	}
}

void Transform::SetWorldRotationQuat(quat rot)
{
	if(owner->GetParent())
	{
		SetRotation(degrees(eulerAngles(rot)) - owner->GetParent()->transform->GetWorldRotation());
	}
	else
	{
		SetRotationQuat(rot);
	}
}

vec3 Transform::GetWorldRotation()
{
	if(owner->GetParent())
	{
		return owner->GetParent()->transform->GetWorldRotation() + GetRotation();
	}
	else
	{
		return rotation;
	}
}

void Transform::SetWorldRotation(vec3 rot)
{
	if(owner->GetParent())
	{
		SetRotation(rot - owner->GetParent()->transform->GetWorldRotation());
	}
	else
	{
		SetRotation(rot);
	}
}

vec3 Transform::GetWorldScale()
{
	if(owner->GetParent())
	{
		return owner->GetParent()->transform->GetWorldScale() * GetScale();
	}
	else
	{
		return GetScale();
	}
}

void Transform::SetWorldScale(vec3 scl)
{
	if(owner->GetParent())
	{
		SetScale(scl - owner->GetParent()->transform->GetWorldScale());
	}
	else
	{
		SetScale(scl);
	}
}

void Transform::Update()
{
//	GetGame()->Log("Transform updating");
//	if(updateMatrix)
	{
		SetMatrix();
	//	updateMatrix = false;
	}
//	SetDirection();
}

void Transform::SetMatrix()
{
//	vec3 scl = vec3(scale.x, scale.z, scale.y);
	vec3 scl = GetScale();
	if(scl == vec3(0))
	{
		matrix = mat4(0);
		return;
	}

//	vec3 pos = vec3(-position.x, position.y, position.z);
	vec3 pos = GetPosition();
	quat rot = GetRotationQuat();
//	vec3 rot = radians(GetRotation());
//	matrix = mat4(1);

	if(owner->GetParent())
	{
		matrix = owner->GetParent()->transform->matrix;
	}
	else
	{
		matrix = mat4(1);
	//	matrix = glm::rotate(matrix, -90.0f, vec3(1,0,0));
	}

//	matrix = glm::translate(mat4(), pos) * mat4_cast(rot) * glm::scale(mat4(), scl);

	if(pos != vec3(0))
	{
		matrix = glm::translate(matrix, pos);
	}

//	if(rot != vec3(0))
	{
	/*	matrix = glm::rotate(matrix, rot.y, vec3(0,1,0));
		matrix = glm::rotate(matrix, rot.z, vec3(0,0,1));
		matrix = glm::rotate(matrix, rot.x, vec3(1,0,0));*/
		matrix *= mat4_cast(rot);
	}

//	if(scl != vec3(1))
	{
		matrix = glm::scale(matrix, scl);
	}

//	mat4 copy = matrix;
//	matrix[2] = copy[1];
//	matrix[1] = copy[2];

/*	if(owner->parent)
	{
		matrix = owner->parent->transform->GetMatrix() * matrix;
	//	matrix = matrix * owner->parent->transform->GetMatrix();
	}*/
}

const mat4& Transform::GetMatrix()
{
//	mat4 matrix = this->matrix;
//	matrix[3].x = -matrix[3].x;
//	mat4 copy = matrix;
//	copy[2] = matrix[1].xyzw();
//	copy[1] = matrix[2].xyzw();
	return matrix;
}

void Transform::SetDirection()
{
/*
	vec3 rot = radians(GetWorldRotation());

//	rot.y = -rot.y;
//	rot.x = -rot.x;

	direction = vec3(
		cos(rot.x) * sin(rot.y),
		sin(rot.x),
		cos(rot.x) * cos(rot.y)
		);
//	direction.x = -direction.x;

	right = vec3(
		sin(rot.y - pi / 2.0f),
		0,
		cos(rot.y - pi / 2.0f)
		);
*/
//	mat4 matrix = GetWorldMatrix();
//	right = vec3(matrix[0].xyz);
//	up = vec3(matrix[1].xyz);
//	direction = vec3(matrix[2].xyz);

//	right = vec3(matrix[0][0], matrix[1][0], matrix[2][0]);
//	up = vec3(matrix[0][1], matrix[1][1], matrix[2][1]);
//	direction = vec3(matrix[0][2], matrix[1][2], matrix[2][2]);
//	up = cross(right, direction);
}

void Transform::Translate(vec3 pos)
{
//	GetGame()->Log("Translating by ", lexical_cast<string>(vec3(-pos.x, pos.y, pos.z)));
	SetPosition(GetPosition() + pos);
}

/// @todo Switch fully to quaternions
void Transform::Rotate(vec3 rot)
{
//	SetRotation(GetRotation() + rot);

	Rotate({1,0,0}, rot.x);
	Rotate({0,1,0}, rot.y);
	Rotate({0,0,1}, rot.z);

	//rotationQuat = quat(rotation);
	//_rotationQuat = rotate( _rotationQuat, rot.x, xdir );
	//_rotationQuat = rotate( _rotationQuat, rot.y, ydir );
	//_rotationQuat = rotate( _rotationQuat, rot.z, zdir );
//	TriggerEvent("SetRotation", rotation);
}

void Transform::Rotate(vec3 axis, float value)
{
	rotationQuat = glm::rotate(rotationQuat, radians(value), axis);
	rotation = eulerAngles(rotationQuat);
}

void Transform::Scale(vec3 scl)
{
	SetScale(GetScale() * scl);
//	TriggerEvent("SetScale", scale);
}

vec3 Transform::GetDirection()
{
	return matrix[2].xyz();
//	return matrix[1].xyz;
}

vec3 Transform::GetRight()
{
	return vec3(matrix[0].xyz());
}

vec3 Transform::GetUp()
{
	return matrix[1].xyz();
//	return matrix[2].xyz;
}

}
