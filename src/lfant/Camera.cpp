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

void Camera::Serialize(Properties* prop)
{
	Component::Serialize(prop);

	prop->Value("fov", &fov);
	prop->Value("aspectRatio", &aspectRatio);
	prop->Value("viewRange", &viewRange);
	prop->Value("aperture", &aperture);
	prop->Value("focalLength", &focalLength);
	prop->Value("focalDepth", &focalDepth);
	prop->Value("useDof", &useDof);
//	prop->Value("mode", &mode);
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
	GetGame()->Log("Updated projection");
	UpdateProjection();

	if(!GetGame()->scene->mainCamera)
	{
		GetGame()->scene->mainCamera = this;
	}
}

void Camera::Update()
{
	UpdateView();
//	UpdateProjection();

	focalLength += GetGame()->time->deltaTime * GetGame()->input->GetAxis("SetFocalLength")->GetValue();
	focalDepth += GetGame()->time->deltaTime * GetGame()->input->GetAxis("SetFocalDepth")->GetValue();
	aperture += GetGame()->time->deltaTime * GetGame()->input->GetAxis("SetFstop")->GetValue();

	if(GetGame()->input->GetButtonDown("ShowDof"))
	{
		GetGame()->Log("focalLength: ", focalLength, ", focalDepth: ", focalDepth, ", aperture: ", aperture);
	}
}

void Camera::Deinit()
{
	GetGame()->Log("mainCamera = ", GetGame()->scene->mainCamera);
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
			GetGame()->Log("projection: ", projection);
			break;
		}
		case Mode::Orthographic:
		{
			float w = (fov/aperture * aspectRatio)*0.5f;
			float h = (fov/aperture / aspectRatio)*0.5f;
			projection = glm::ortho(w, -w, h, -h, viewRange.min, viewRange.max);
			break;
		}
	}
}

const mat4& Camera::GetProjection()
{
	return projection;
}

const mat4& Camera::GetView()
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
