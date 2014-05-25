/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
#pragma once
#include <lfant/stdafx.h>

// External
#include <glm/mat4x4.hpp>

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

/**
 *
 */
class Camera : public Component
{
	friend class Renderer;
public:
	Camera();
	Camera(const Camera& other);
	virtual ~Camera();

	virtual Component* Clone(Entity* owner) const;

	virtual void Init();
	virtual void Update();
	virtual void Deinit();

	
	virtual void Serialize(Properties* prop);

	/**
	 *	Updates the projection to use the current settings
	 */
	void UpdateProjection();

	const mat4& GetProjection();
	const mat4& GetView();

	void UpdateView();
	void OnViewChange(vec3 change)
	{
		UpdateView();
	}

	/**	Sets the Field Of View for this Camera's perspective.
	 */
	void SetFOV(float fov);
	float GetFOV();

	/**	Sets the Aspect Ratio to use.
	 */
	void SetAspectRatio(float ratio);
	float GetAspectRatio();

	/**	Sets the viewing range to use (near and far).
	 */
	void SetViewRange(float near, float far);
	Range<float> GetViewRange();

	void SetAperture(float value);
	float GetAperture();

	vec3 lastPos;

	Rect viewport = {0.0f, 0.0f, 1.0f, 1.0f};

	enum class Mode : short
	{
		Perspective = 0,
		Orthographic = 1
	};
	Mode mode = Mode::Perspective;

	// Include film size for DoF and part of FoV?
//	float filmSize = 35.0f;

	bool autoFocus = false;
	vec2 focusPoint;

	float focalLength = 35.0f;
	float focalDepth = 6.6f;
	float aperture = 4.0f;
	float focus = 0.9f;
	bool useDof = false;

protected:

	/// Matrix for specific projection.
	mat4 projection;

	/// Matrix for transformations.
	mat4 view;

	/// Field of view, eg. 40.0f
	float fov = 66.0f;

	/// Aspect ratio, eg. 16.0f/9.0f
	float aspectRatio = 16.0f / 9.0f;

	/// The viewing range (near and far) of this camera's view, eg. (0.1f, 1000.0f)
	Range<float> viewRange = { 0.01f, 1000.0f };

	// Real Camera settings

	float shutterSpeed = 0.003f;

	uint16 iso = 300;


private:

};

/** @} */
/** @} */
}
