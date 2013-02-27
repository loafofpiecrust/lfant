
#include <lfant/Shader.hpp>

// External
#include <GL/glew.h>

// Internal

#include <lfant/Console.hpp>
#include <lfant/FileSystem.hpp>

namespace lfant
{

void Shader::LoadFile(string path)
{
	string folder = game->fileSystem->gameFolder + "/assets/";
	string vert = folder + path + ".vert";
	string frag = folder + path + ".frag";
	FILE* vertFile = fopen(vert.c_str(), "rb");
	FILE* fragFile = fopen(frag.c_str(), "rb");
	Log("LoadShader: Files opened");
	if (!vertFile || !fragFile)
	{
		Log("Shader::LoadFile: " + path + " could not be loaded");
		return;
	}

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

	// Link the program
	Log("Shader::LoadFile: Linking shader");
	uint32_t ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	char ProgramErrorMessage[glm::max(InfoLogLength, int(1))];
	glGetProgramInfoLog(ProgramID, InfoLogLength, 0, &ProgramErrorMessage[0]);
	if (InfoLogLength > 0)
	{
		Log(ProgramErrorMessage);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
	name = path;
	id = ProgramID;
}

}
