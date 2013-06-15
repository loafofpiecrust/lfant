
#include <lfant/physics/DebugRenderer.h>

// Internal
#include <lfant/Game.h>
#include <lfant/Scene.h>
#include <lfant/Camera.h>
#include <lfant/Console.h>

// External
#include <GL/glew.h>

namespace lfant {
namespace physics {

DebugRenderer::DebugRenderer()
{
//	points.push_back(btVector3(0,0,0));
//	points.push_back(btVector3(0,0,0));

	material = new Material;
	material->LoadFile("materials/lines.mat");

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	if(material->shader->GetId() == 0)
	{
		material->shader->LoadFile();
	}
	if(material->shader->GetId() != 0)
	{
		Log("Adding uniforms..");
		material->shader->AddUniform("matrix");
		material->shader->AddUniform("color");
	}

	glGenBuffers(1, &pointBuffer);
//	glBindBuffer(GL_ARRAY_BUFFER, pointBuffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(uint32_t)*points.size(), &points[0], GL_STATIC_DRAW);
}

DebugRenderer::~DebugRenderer()
{
}

void DebugRenderer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	points.push_back(from);
	points.push_back(to);
}

void DebugRenderer::Render()
{
	glBindBuffer(GL_ARRAY_BUFFER, pointBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uint32_t)*points.size(), &points[0], GL_STATIC_DRAW);

	glBindVertexArray(vertexArray);
	material->shader->Bind();

	mat4 mvp = game->scene->mainCamera->GetProjection() * game->scene->mainCamera->GetView();
	glUniformMatrix4fv(material->shader->GetUniform("matrix"), 1, GL_FALSE, &mvp[0][0]);
//	glUniform3f(material->shader->GetUniform("color"), color[0], color[1], color[2]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, pointBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_LINES, 0, points.size());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	material->shader->Unbind();

	points.clear();
}

}
}