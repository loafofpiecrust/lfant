
#include <lfant/Shader.h>

// Internal
#include <lfant/Console.h>
#include <lfant/FileSystem.h>
#include <lfant/Game.h>
#include <lfant/Renderer.h>
#include <lfant/FileSystem.h>
#include <lfant/Texture.h>

// External
#include <GL/glew.h>

namespace lfant
{

deque<Shader*> Shader::shaders;
Shader* Shader::current;

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
	Log("Shader::LoadFile: Touch.");
	if(file != "")
	{
		vertex = file+".vert";
		fragment = file+".frag";
	}
	for(auto& sh : shaders)
	{
		if(vertex == sh->vertex && fragment == sh->fragment && sh->id != 0)
		{
			Log("Shader::LoadFile: Found previous shader that's compatible for '", vertex, "'.");
			id = sh->id;
			return;
		}
	}
	path vert = game->fileSystem->GetGamePath(vertex);
	path frag = game->fileSystem->GetGamePath(fragment);
	Log("LoadShader: Files opened, of ", file);
	if (!exists(vert) || !exists(frag))
	{
		Log("Shader::LoadFile: " + file + " could not be loaded");
		return;
	}
	
	Log("Shader::LoadFile: Compiling shader");
	uint32 vertId = Compile(GL_VERTEX_SHADER, vert.string());
	uint32 fragId = Compile(GL_FRAGMENT_SHADER, frag.string());

	// Link the program
	Log("Shader::LoadFile: Linking shader");
	uint32_t ProgramID = glCreateProgram();
	glAttachShader(ProgramID, vertId);
	glAttachShader(ProgramID, fragId);
	glLinkProgram(ProgramID);
	id = ProgramID;
	Log("Shader::LoadFile: Shader id '", id, "'.");

	// Check the program
	CheckErrors();

	glDeleteShader(vertId);
	glDeleteShader(fragId);
//	path = file;

	shaders.push_back(this);
	Unbind();
}

uint32 Shader::GetUniform(string name)
{
	return uniforms[name];
}

void Shader::AddUniform(string name)
{
	uniforms[name] = glGetUniformLocation(id, name.c_str());
//	Log("Adding uniform '"+name+"' as ", uniforms[name]);
}

void Shader::Bind()
{
	glUseProgram(id);
	Shader::current = this;
//	Log("Shader, binding ", id);
}

void Shader::Unbind()
{
	glUseProgram(0);
	Shader::current = nullptr;
//	Log("Shader, unbinding ", id);
}

Shader* Shader::GetCurrent()
{
	return Shader::current;
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

void Shader::CheckErrors()
{
	int32 result;
	int32 logLength;
	glGetProgramiv(id, GL_LINK_STATUS, &result);
	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);
	char ProgramErrorMessage[glm::max(logLength, int(1))];
	glGetProgramInfoLog(id, logLength, 0, &ProgramErrorMessage[0]);
	if (logLength > 0)
	{
		Log(ProgramErrorMessage);
	}
}

void Shader::SetUniform(string name, float val)
{
	glUniform1f(GetUniform(name), val);
}

void Shader::SetUniform(string name, const vec2& val)
{
	glUniform2f(GetUniform(name), val.x, val.y);
}

void Shader::SetUniform(string name, const vec3& val)
{
	glUniform3f(GetUniform(name), val.x, val.y, val.z);
}

void Shader::SetUniform(string name, const vec4& val)
{
	glUniform4f(GetUniform(name), val.x, val.y, val.z, val.w);
}

void Shader::SetUniform(string name, const mat4& val)
{
	glUniformMatrix4fv(GetUniform(name), 1, GL_FALSE, &val[0][0]);
}

void Shader::SetUniform(string name, Texture* val)
{
	val->Bind();
	glUniform1i(GetUniform(name), 0);
//	val->Unbind();
}

}
