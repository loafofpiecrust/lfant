#include "Mesh.h"

#include "lfant/Geometry.h"
#include "lfant/Material.h"
#include "lfant/Scene.h"
#include "lfant/Camera.h"
#include "lfant/Transform.h"
#include "lfant/Texture.h"
#include "lfant/Console.h"
#include "lfant/FileSystem.h"

namespace lfant
{

//static const TypeRegistry<Component>::Entry<Mesh> type_registryEntry_ {Component::typeRegistry, "Mesh", "Renderable"};
IMPLEMENT_SUBTYPE(Component, Mesh, Renderable)

Mesh::Mesh()
{
	render = true;
}

void Mesh::Load(Properties *prop)
{
	Renderable::Load(prop);

	string meshFile = prop->Get("file");
	if(!meshFile.empty())
	{
		GetGame()->Log("Loading mesh fillet");
		mesh = Geometry::LoadFile(GetGame()->GetAssetPath(meshFile).string());
	}

	if(Properties* child = prop->GetChild("material"))
	{
		material.Load(child);
	}
	else
	{
		string matFile = prop->Get("material");
		if(!matFile.empty()) material.LoadFile(matFile);
	}

	Init();
}

void Mesh::Save(Properties *prop) const
{
	Renderable::Save(prop);
}

void Mesh::Init()
{
	Renderable::Init();

	GetGame()->Log("Mesh::Init()");

	if(!mesh)
	{
		GetGame()->Log("No mesh. Disabling");
	//	Enable(false);
		return;
	}

	if(!material.shader || !material.texture)
	{
		material.LoadFile(GetGame()->GetAssetPath("materials/Diffuse.prop").string());
	}

//	material.shader->Bind();

	material.shader->AddUniform("M");
	material.shader->AddUniform("V");
	material.shader->AddUniform("P");
	material.shader->AddUniform("textureSampler");
	material.shader->AddUniform("tiling");
}

void Mesh::Render()
{
	if(!mesh)
	{
		return;
	}

//	glBindVertexArray(mesh->vertexArray);
	material.shader->Bind();

	if(owner && usingCamera)
	{
		material.shader->SetUniform("P", GetGame()->scene->mainCamera->GetProjection());
		material.shader->SetUniform("V", GetGame()->scene->mainCamera->GetView());
		material.shader->SetUniform("M", owner->transform->GetMatrix());
	}

	if(material.texture)
	{
	//	GetGame()->Log("Setting texture uniforms.");
		material.shader->SetUniform("tiling", material.tiling);

		material.shader->SetUniform("textureSampler", material.texture.get());
	}

	mesh->Render();

//	material.texture->Unbind();
//	glUseProgram(0);
//	glBindVertexArray(0);
}

void Mesh::Deinit()
{
	mesh.reset();
}

} // namespace lfant
