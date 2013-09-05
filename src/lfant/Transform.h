/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
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
#include <lfant/stdafx.h>

// Internal
#include <lfant/Component.h>

// External

namespace lfant {

class Transform : public Component
{
public:
	Transform();
	virtual ~Transform();

	virtual void Save(Properties* prop);
	virtual void Load(Properties* prop);

	virtual void Init();
	virtual void Update();

	vec3 GetPosition();
	void SetPosition(vec3 pos);
	vec3 GetWorldPosition();
	void SetWorldPosition(vec3 pos);

	quat GetRotationQuat();
	void SetRotationQuat(quat q);
	quat GetWorldRotationQuat();
	void SetWorldRotationQuat(quat q);

	vec3 GetRotation();
	void SetRotation(vec3 rot);
	vec3 GetWorldRotation();
	void SetWorldRotation(vec3 rot);

	vec3 GetScale();
	void SetScale(vec3 scl);
	vec3 GetWorldScale();
	void SetWorldScale(vec3 scl);

	void Translate(vec3 pos);
	void Rotate(vec3 rot);
	void Scale(vec3 scl);

	void SetDirection();
	void SetMatrix();
	mat4 GetMatrix();

	vec3 GetDirection();
	vec3 GetRight();
	vec3 GetUp();

protected:

	void OnSetWorldPos();

	vec3 position = vec3(0);
	vec3 rotation = vec3(0);
	vec3 scale = vec3(1);
	quat rotationQuat;
	mat4 matrix = mat4(1);

	vec3 up;
	vec3 right;
	vec3 direction;

	Transform* parent = nullptr;
	bool updateMatrix = false;


private:

};

}
