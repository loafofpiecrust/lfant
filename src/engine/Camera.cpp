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
 *		http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 *
 ******************************************************************************/

#include "Camera.hpp"

// External

// Internal
#include "Transform.hpp"
#include "MathUtil.hpp"
#include "Console.hpp"

namespace sfs
{

Camera::Camera()
{
}

Camera::~Camera()
{
}

/*******************************************************************************
 *
 *		Generic Entity functions
 *
 *******************************************************************************/

void Camera::Init()
{
	UpdateProjection();
}

void Camera::Update()
{
	transform->SetDirection();
	UpdateView();
	UpdateProjection();
}

/*******************************************************************************
 *
 *		Projection functions
 *
 *******************************************************************************/

void Camera::SetProjection(float fov, float aspect, float min, float max)
{
	switch(projectionMode)
	{
		case PM_PERSPECTIVE:
			projection = perspective(fov, aspect, min, max);
			break;
		case PM_ORTHO:
			projection = ortho(-fov / 2 * aspect, fov / 2 * aspect, -fov / 2 / aspect, fov / 2 / aspect, min, max);
			break;
	}
	this->fov = fov;
	this->aspectRatio = aspect;
	this->viewRange.min = min;
	this->viewRange.max = max;
}

void Camera::UpdateProjection()
{
	SetProjection(fov, aspectRatio, viewRange.min, viewRange.max);
}

void Camera::UpdateView()
{
	/// @todo Optimize this process? Fix for components
	view = lookAt(transform->worldPosition(), transform->worldPosition() + transform->direction, transform->up);
}

void Camera::SetFOV(float fov)
{
	this->fov = fov;
	UpdateProjection();
}

void Camera::SetRatio(float ratio)
{
	aspectRatio = ratio;
	UpdateProjection();
}

void Camera::SetRange(float min, float max)
{
	viewRange.min = min;
	viewRange.max = max;
	UpdateProjection();
}

}
