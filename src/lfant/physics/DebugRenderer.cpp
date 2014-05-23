
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

DebugRenderer::DebugRenderer() :
	mode(btIDebugDraw::DBG_DrawWireframe)
{
//	points.push_back(btVector3(0,0,0));
//	points.push_back(btVector3(0,0,0));

	material = new Material;
//	material->LoadFile("materials/lines.mat");

	if(!material->shader)
	{
		material->shader = Shader::LoadFile("shaders/lines/Default.vert", "shaders/lines/Default.frag");
	}
	if(material->shader)
	{
	//	GetGame()->Log("Adding uniforms..");
		material->shader->AddUniform("matrix");
		material->shader->AddUniform("color");
	}

	glGenBuffers(1, &pointBuffer);
	glGenBuffers(1, &colorBuffer);
//	glBindBuffer(GL_ARRAY_BUFFER, pointBuffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(uint32)*points.size(), &points[0], GL_STATIC_DRAW);
}

DebugRenderer::~DebugRenderer()
{
}

void DebugRenderer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
//	std::cout << "from: " <<from.getX()<<","<<from.getY()<<","<<from.getZ()<<"\n";
	points.push_back(from);
	points.push_back(to);
	colors.push_back(color);
	colors.push_back(color);

/*	float tmp[ 6 ] = { from.getX(), from.getY(), from.getZ(),
                    to.getX(), to.getY(), to.getZ() };
      
      glPushMatrix();
      {         
         glColor4f(color.getX(), color.getY(), color.getZ(), 1.0f);         
         glVertexPointer( 3,
                     GL_FLOAT,
                     0,
                     &tmp );
         
         glPointSize( 5.0f );
         glDrawArrays( GL_POINTS, 0, 2 );
         glDrawArrays( GL_LINES, 0, 2 );
      }
      glPopMatrix();*/
}

void DebugRenderer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	return;
}

void DebugRenderer::reportErrorWarning(const char* warningString)
{
//	GetGame()->Log("Bullet error: '", warningString, "'");
}

void DebugRenderer::draw3dText(const btVector3& location, const char* textString)
{
	return;
}

void DebugRenderer::setDebugMode(int debugMode)
{
	this->mode = debugMode;
}

int DebugRenderer::getDebugMode() const
{
	return mode;
}

void DebugRenderer::Render(Game* game)
{
	glBindBuffer(GL_ARRAY_BUFFER, pointBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(btVector3)*points.size(), &points[0], GL_STREAM_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(btVector3)*colors.size(), &colors[0], GL_STREAM_DRAW);

	material->shader->Bind();

	mat4 mvp = game->scene->mainCamera->GetProjection() * game->scene->mainCamera->GetView();
//	mat4 mvp(1);
	material->shader->SetUniform("matrix", mvp);
//	glUniform3f(material->shader->GetUniform("color"), color[0], color[1], color[2]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, pointBuffer);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_LINES, 0, points.size());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	material->shader->Unbind();

	points.clear();
	colors.clear();
//	points.resize(0);
}

}
}
