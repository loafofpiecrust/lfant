/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2013-06-12 by Taylor Snead
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
#include <lfant/Object.h>
#include <lfant/Texture.h>
#include <lfant/Rect.h>
#include <lfant/Shader.h>

// External

namespace lfant {

class FrameBuffer : public Object
{
public:
	FrameBuffer();
	~FrameBuffer();

	void Init();
	void OnDestroy();

	void Render();

	static void Clear();

	void Bind();
	static void Unbind();

	static FrameBuffer* GetCurrent();

	uint32_t GetAttachment(uint32_t idx);

	void ResizeViewport();
	void SetRect(URect r) { rect = r; }
	void Resize();

	void AddTexture(string name, Texture* tex = nullptr);
	void GetTextures(Shader* sh);

	URect rect {0,0,0,0};
	bool hasDepth = false;
//	ptr<Shader> shader;

protected:
	uint32_t id;

	deque<string> textureNames;
	deque<Texture*> textures;

	vector<uint32_t> drawBuffers;
	uint32_t depthBuffer;

	static FrameBuffer* current;

private:
};

}