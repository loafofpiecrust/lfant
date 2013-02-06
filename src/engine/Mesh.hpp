/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
 *	Created: 2012-07-19 by Taylor Snead
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
#include "stdafx.hpp"

// External

// Internal
#include "Material.hpp"
#include "Vertex.hpp"
#include "Component.hpp"

namespace sfs
{
	/** @addtogroup Engine
	 *	@{
	 */
	/** @addtogroup Rendering
	 *	@{
	 */

	/**	The Mesh class that holds a 3D mesh and possibly an animated skeleton.
	 *	@remarks
	 *		This class controls a Mesh as defined by the 3d importing library.
	 *		It handles holding a list of vertices, bones, faces, etc. It also
	 *		holds a list of animations and things involving that. Keep in mind
	 *		that this class isn't used directly for MeshRendering for Entities.
	 *		The MeshRenderer class is used for that, and MeshRenderer has functions
	 *		to manage changing the model and animset of the Mesh, and for attaching
	 *		children mesh to it (possible, not sure yet)
	 *	@todo
	 *		Be sure this works fully in conjuction with the importers Mesh classes.
	 *		Have this display as-is (ONLY for debug purposes; Later, require an Entity with MeshRenderer and SceneNode)
	 */
	class Mesh : public Component
	{
		friend class Renderer;
	public:
		Mesh();
		~Mesh();

		virtual void Init();
		virtual void Update();
		virtual void OnDestroy();

		void SetInput(string path);
		void SetTexture(string name);

		vector<Vertex> vertices;
		vector<uint32_t> indices;

		Material material;

	protected:
		string file = "";

		uint32_t vertexBuffer = 0;
		uint32_t indexBuffer = 0;

		uint32_t matrixId = 0;
		bool castShadows = true;
		bool receiveShadows = true;

		bool initialized = false;

	private:
	};

/// @}
/// @}
}
