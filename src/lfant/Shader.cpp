
#include <lfant/Shader.h>

// External
#include <GL/glew.h>

// Internal
#include <lfant/Console.h>
#include <lfant/FileSystem.h>
#include <lfant/Game.h>
#include <lfant/Renderer.h>
#include <lfant/FileSystem.h>

namespace lfant
{

void Shader::Load(Properties *prop)
{
	prop->Get("vertex", vertex);
	prop->Get("fragment", fragment);
	LoadFile();
}

void Shader::Save(Properties *prop)
{
	prop->Set("vertex", vertex);
	prop->Set("fragment", fragment);
}

void Shader::LoadFile(string file)
{
	string vert = game->fileSystem->GetGamePath(vertex).string();
	string frag = game->fileSystem->GetGamePath(fragment).string();
	FILE* vertFile = fopen(vert.c_str(), "rb");
	FILE* fragFile = fopen(frag.c_str(), "rb");
	Log("LoadShader: Files opened");
	if (!vertFile || !fragFile)
	{
		Log("Shader::LoadFile: " + file + " could not be loaded");
		return;
	}

	/*
	// Create the shaders
	uint32_t VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	uint32_t FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	Log("Shader::LoadFile: Shaders created");

	// Read the Vertex Shader code from the file
	string VertexShaderCode;
	ifstream VertexShaderStream(vert, ios::in);
	if (VertexShaderStream.is_open())
	{
		string Line = "";
		while (getline(VertexShaderStream, Line))
		{
			VertexShaderCode += "\n" + Line;
		}
		VertexShaderStream.close();
	}

	Log("Shader::LoadFile: Vertex shader read");

	// Read the Fragment Shader code from the file
	string FragmentShaderCode;
	ifstream FragmentShaderStream(frag, ios::in);
	if (FragmentShaderStream.is_open())
	{
		string Line = "";
		while (getline(FragmentShaderStream, Line))
		{
			FragmentShaderCode += "\n" + Line;
		}
		FragmentShaderStream.close();
	}

	Log("Shader::LoadFile: Fragment shader read");

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	Log("Shader::LoadFile: Compiling shader : " + vert);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, 0);
	glCompileShader(VertexShaderID);

	Log("Shader::LoadFile: Vertex shader compiled");

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	Log("Shader::LoadFile: Retrieved compilation info");
	char VertexShaderErrorMessage[InfoLogLength];
	glGetShaderInfoLog(VertexShaderID, InfoLogLength, 0, &VertexShaderErrorMessage[0]);
	Log("Shader::LoadFile: Retrieved compilation log");
	if (InfoLogLength > 0)
	{
		Log(VertexShaderErrorMessage);
	}

	Log("Shader::LoadFile: Vertex shader checked");

	// Compile Fragment Shader
	Log("Shader::LoadFile: Compiling shader : " + frag);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, 0);
	glCompileShader(FragmentShaderID);

	Log("Shader::LoadFile: Fragment shader compiled");

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	char FragmentShaderErrorMessage[InfoLogLength];
	glGetShaderInfoLog(FragmentShaderID, InfoLogLength, 0, &FragmentShaderErrorMessage[0]);
	if (InfoLogLength > 0)
	{
		Log(FragmentShaderErrorMessage);
	}

	Log("Shader::LoadFile: Fragment shader checked");

	*/
	int32 result;
	int32 logLength;
	uint32 vertId = Compile(GL_VERTEX_SHADER, vert);
	uint32 fragId = Compile(GL_FRAGMENT_SHADER, frag);

	// Link the program
	Log("Shader::LoadFile: Linking shader");
	uint32_t ProgramID = glCreateProgram();
	glAttachShader(ProgramID, vertId);
	glAttachShader(ProgramID, fragId);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &logLength);
	char ProgramErrorMessage[glm::max(logLength, int(1))];
	glGetProgramInfoLog(ProgramID, logLength, 0, &ProgramErrorMessage[0]);
	if (logLength > 0)
	{
		Log(ProgramErrorMessage);
	}

	glDeleteShader(vertId);
	glDeleteShader(fragId);
//	path = file;
	id = ProgramID;

	game->renderer->AddShader(this);
}

uint32 Shader::GetUniform(string name)
{
	return uniforms[name];
}

void Shader::AddUniform(string name)
{
	uniforms[name] = 0;
	uniforms[name] = glGetUniformLocation(id, name.c_str());
	Log("Adding uniform '"+name+"' as ", uniforms[name]);
}

void Shader::Use()
{
	if(id != 0)
	{
		glUseProgram(id);
	}
}

void Shader::Unuse()
{
	glUseProgram(0);
}

uint32 Shader::GetId()
{
	return id;
}

uint32 Shader::Compile(uint32 type, const string &path)
{
	ifstream stream(path);
	string line = "";
	string source = "";

	while(stream.good())
	{
		getline(stream, line);
		source += "\n"+line;
	}
	stream.close();

	uint32 shader = glCreateShader(type);
	const char* csource = source.c_str();
	glShaderSource(shader, 1, &csource, 0);
	glCompileShader(shader);

	int32 logLength = 0;
	int32 result = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	char error[logLength];
	glGetShaderInfoLog(shader, logLength, 0, &error[0]);
	if (logLength > 0)
	{
		Log(error);
	}

	return shader;
}

}
