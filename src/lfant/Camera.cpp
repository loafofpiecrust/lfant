/*
*	Copyright (C) 2013-2014, by loafofpiecrust
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

#include <lfant/Camera.h>

// External
#include <glm/gtx/projection.hpp>
#include <glm/detail/func_matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Internal
#include <lfant/Transform.h>
#include <lfant/util/Math.h>
#include <lfant/Console.h>
#include <lfant/Scene.h>
#include <lfant/Time.h>
#include <lfant/Input.h>

namespace lfant {

IMPLEMENT_TYPE(Component, Camera)

Camera::Camera()
{
}

Camera::Camera(const Camera& other) :
	Component(other)
{
}

Camera::~Camera()
{
}

Component* Camera::Clone(Entity* owner) const
{
	Camera* inst = new Camera;
	Component::Clone(inst, owner);
	return inst;
}

void Camera::Save(Properties* prop) const
{
	Component::Save(prop);

	prop->Set("fov", fov);
	prop->Set("aspectRatio", aspectRatio);
	prop->Set("viewRange", viewRange);
	prop->Set("aperture", aperture);
	prop->Set("focalLength", focalLength);
	prop->Set("focalDepth", focalDepth);
//	prop->Set("mode", (short)mode);
}

void Camera::Load(Properties* prop)
{
	Component::Load(prop);

	prop->Get("fov", fov);
	prop->Get("aspectRatio", aspectRatio);
	prop->Get("viewRange", viewRange);
	prop->Get("aperture", aperture);
	prop->Get("focalLength", focalLength);
	prop->Get("focalDepth", focalDepth);
//	mode = (Mode)prop->Get<short>("mode");

	UpdateProjection();
}

/*******************************************************************************
*
*		Generic Entity functions
*
*******************************************************************************/

void Camera::Init()
{
	Log("Updated projection");
	UpdateProjection();

	if(!game->scene->mainCamera)
	{
		game->scene->mainCamera = this;
	}
}

void Camera::Update()
{
	UpdateView();
//	UpdateProjection();

	focalLength += game->time->deltaTime * game->input->GetAxis("SetFocalLength")->GetValue();
	focalDepth += game->time->deltaTime * game->input->GetAxis("SetFocalDepth")->GetValue();
	aperture += game->time->deltaTime * game->input->GetAxis("SetFstop")->GetValue();

	if(game->input->GetButtonDown("ShowDof"))
	{
		Log("focalLength: ", focalLength, ", focalDepth: ", focalDepth, ", aperture: ", aperture);
	}
}

void Camera::Deinit()
{
	Log("mainCamera = ", game->scene->mainCamera);
}

/*******************************************************************************
*
*		Projection functions
*
*******************************************************************************/

void Camera::UpdateProjection()
{
	switch(mode)
	{
		case Mode::Perspective:
		{
			projection = glm::perspective(fov/aperture, aspectRatio, viewRange.min, viewRange.max);
		//	projection[10] = -projection[10];
			Log("projection: ", projection);
			break;
		}
		case Mode::Orthographic:
		{
			projection = glm::ortho(0.0f, fov * aspectRatio, 0.0f, fov / aspectRatio, viewRange.min, viewRange.max);
			break;
		}
	}
}

mat4 Camera::GetProjection()
{
	return projection;
}

mat4 Camera::GetView()
{
	return view;
}

void Camera::UpdateView()
{
	vec3 pos = owner->transform->GetWorldPosition();
	view = glm::lookAt(
				pos,
				pos + owner->transform->GetDirection(),
				owner->transform->GetUp()
				);
}

void Camera::SetFOV(float fov)
{
	this->fov = fov;
	UpdateProjection();
}

float Camera::GetFOV()
{
	return fov;
}

void Camera::SetAspectRatio(float ratio)
{
	aspectRatio = ratio;
	UpdateProjection();
	TriggerEvent("SetAspectRatio", aspectRatio);
}

float Camera::GetAspectRatio()
{
	return aspectRatio;
}

void Camera::SetViewRange(float min, float max)
{
	viewRange.min = min;
	viewRange.max = max;
	UpdateProjection();
}

Range<float> Camera::GetViewRange()
{
	return viewRange;
}

void Camera::SetAperture(float value)
{
	aperture = value;
	UpdateProjection();
}

float Camera::GetAperture()
{
	return aperture;
}

}
