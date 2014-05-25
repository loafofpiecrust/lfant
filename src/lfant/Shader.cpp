
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
#include <boost/range/algorithm/remove_if.hpp>
#include <glm/glm.hpp>

using namespace std;

namespace lfant
{

deque<std::weak_ptr<Shader>> Shader::shaderCache;
Shader* Shader::current;

Shader::Shader()
{
}

Shader::~Shader()
{
	glDeleteProgram(id);
}

std::shared_ptr<Shader> Shader::Load(Properties *prop)
{
	string vertex="", fragment="", geometry="";
	prop->Value("vertex", &vertex);
	prop->Value("fragment", &fragment);
	prop->Value("geometry", &geometry);
	return LoadFile(vertex, fragment, geometry);
}

void Shader::Serialize(Properties *prop)
{
	prop->Value("vertex", vertex);
	prop->Value("fragment", fragment);
	prop->Value("geometry", geometry);
}

std::shared_ptr<Shader> Shader::LoadFile(string vert, string frag, string geom, string comp)
{
	for(auto& sh_weak : shaderCache)
	{
		auto sh = sh_weak.lock();
		if(sh->vertex == vert && sh->fragment == frag)
		{
			return sh;
		}
	}

	std::shared_ptr<Shader> sh {new Shader};
	shaderCache.push_back(sh);

	sh->vertex = vert;
	sh->fragment = frag;
	sh->geometry = geom;
	sh->compute = comp;

	sh->Compile();

	return sh;
}

void Shader::Compile()
{
	if(vertex.empty() || fragment.empty())
	{
		return;
	}

	uint32 vert = -1;
	uint32 frag = -1;
	id = glCreateProgram();

	if(!vertex.empty())
	{
		vert = Compile(GL_VERTEX_SHADER, vertex);
		glAttachShader(id, vert);
	}
	if(!fragment.empty())
	{
		frag = Compile(GL_FRAGMENT_SHADER, fragment);
		glAttachShader(id, frag);
	}

#if !LFANT_GLES
	uint32 geom = -1;
	uint32 comp = -1;
	if(!geometry.empty())
	{
		geom = Compile(GL_GEOMETRY_SHADER, geometry);
		glAttachShader(id, geom);
	}
	if(!compute.empty())
	{
		comp = Compile(GL_COMPUTE_SHADER, compute);
		glAttachShader(id, comp);
	}
#endif

	glLinkProgram(id);
	CheckErrors();

	if(vert) glDeleteShader(vert);
	if(frag) glDeleteShader(frag);
#if !LFANT_GLES
	if(geom != -1) glDeleteShader(geom);
	if(comp != -1) glDeleteShader(comp);
#endif

	Unbind();

//	GetGame()->Log(glGetError());
}

uint32 Shader::GetUniform(string name)
{
	return uniforms[name];
}

void Shader::AddUniform(string name)
{
	uniforms[name] = glGetUniformLocation(id, name.c_str());
}

void Shader::Bind()
{
	if(GetCurrent() != this)
	{
		glUseProgram(id);
		SetCurrent(this);
	}
}

void Shader::Unbind()
{
	if(GetCurrent() != nullptr)
	{
		glUseProgram(0);
		SetCurrent(nullptr);
	}
}

Shader* Shader::GetCurrent()
{
	return Shader::current;
}

void Shader::SetCurrent(Shader *sh)
{
	Shader::current = sh;
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

	deque<string> str;
	while(stream.good())
	{
		getline(stream, line);
		// extra parsing code?
		if(line[0] == '#')
		{
			str = Split(line, "\t <>\"");
			if(str[0] == "#include")
			{
//				GetGame()->Log("including in shader '"+str[1]+"'.");
				boost::filesystem::path p {path};
				p = {p.remove_filename().string()+"/"+str[1]};
				ifstream stream2(p.string());
				string line = "";
				while(stream2.good())
				{
					getline(stream2, line);
					source += "\n"+line;
				}
				stream2.close();
				continue;
			}
		}
		source += "\n"+line;
	}
	stream.close();

	uint32 shader = glCreateShader(type);
	const char* csource = (source+"\0").c_str();
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
		std::cout << error << "\n";
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
		std::cout << ProgramErrorMessage << "\n";
	}
}

void Shader::SetUniform(string name, float val)
{
	glUniform1f(GetUniform(name), val);
}

void Shader::SetUniform(string name, int val)
{
	glUniform1i(GetUniform(name), val);
}

void Shader::SetUniform(string name, unsigned int val)
{
	glUniform1ui(GetUniform(name), val);
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
	glUniform1i(GetUniform(name), val->GetIndex());
//	val->Unbind();
}

}
