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
#include <lfant/Mesh.h>

// External

namespace lfant {

class FrameBuffer : public Object
{
public:
	FrameBuffer();
	~FrameBuffer();

	virtual void Init();
	virtual void OnDestroy();

	void DrawBuffers();

	static void Clear();

	void Bind();
	static void Unbind();

	void BindRead();
	static void UnbindRead();

	static FrameBuffer* GetCurrent();

	uint32_t GetAttachment(uint32_t idx);

	void ResizeViewport();
	void SetRect(URect r) { rect = r; }
	void Resize();

	void AddTexture(string name, Texture::Format form1 = Texture::Format::RGB32F, Texture::Format form2 = Texture::Format::RGB);
	void AddDepthTexture(string name, Texture::Format form1 = Texture::Format::RGB32F, Texture::Format form2 = Texture::Format::RGB);
	void GetTextures(Shader* sh) const;
	Texture* GetTexture(string name) const;

	void SetCurrentTexture(string name);

	virtual void BeginRender();
	virtual void Render();
	virtual void EndRender();

	URect rect {0,0,0,0};
	bool hasDepth = false;
	uint32_t startIndex = 0;
//	ptr<Shader> shader;

protected:
	uint32_t id = 0;

	deque<string> textureNames;
	deque<Texture*> textures;
	Texture* currentTex = nullptr;
	Texture* depthTexture = nullptr;
	string depthTexName = "";

	Buffer<vec2> posBuffer;
//	Mesh::Buffer<vec2> uvBuffer = 0;
	ptr<Shader> shader;

	vector<uint32_t> drawBuffers;
	uint32_t depthBuffer = 0;

	static FrameBuffer* current;

private:
};

}