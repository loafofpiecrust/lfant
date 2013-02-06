/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
 *	Created: 2013-01-13 by Taylor Snead
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *		http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 *
 ******************************************************************************/

#include "ShaderLoader.hpp"

// External
#include <GL/glew.h>
#include <fstream>

// Internal
#include "Console.hpp"

namespace sfs
{
	Shader LoadShader( string name )
	{
		string vert = name + ".vert";
		string frag = name + ".frag";
		Log("LoadShader: Begin with "+vert+" and "+frag);
		FILE* vertFile = fopen(vert.c_str(), "rb");
		FILE* fragFile = fopen(frag.c_str(), "rb");
		Log("LoadShader: Files opened");
		if (!vertFile || !fragFile)
		{
			Log("LoadShader: " + name + " could not be loaded");
			return null(Shader) ;
		}

		// Create the shaders
		uint32_t VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		uint32_t FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		Log("LoadShader: Shaders created");

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

		Log("LoadShader: Vertex shader read");

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

		Log("LoadShader: Fragment shader read");

		GLint Result = GL_FALSE;
		int InfoLogLength;

		// Compile Vertex Shader
		Log("Compiling shader : " + vert);
		char const* VertexSourcePointer = VertexShaderCode.c_str();
		glShaderSource(VertexShaderID, 1, &VertexSourcePointer, 0);
		glCompileShader(VertexShaderID);

		Log("Vertex shader compiled");

		// Check Vertex Shader
		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		Log("Shader: Retrieved compilation info");
		char VertexShaderErrorMessage[InfoLogLength];
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, 0, &VertexShaderErrorMessage[0]);
		Log("Shader: Retrieved compilation log");
		if (InfoLogLength > 0)
		{
			Log(VertexShaderErrorMessage);
		}

		Log("Vertex shader checked");

		// Compile Fragment Shader
		Log("Compiling shader : " + frag);
		char const* FragmentSourcePointer = FragmentShaderCode.c_str();
		glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, 0);
		glCompileShader(FragmentShaderID);

		Log("Fragment shader compiled");

		// Check Fragment Shader
		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		char FragmentShaderErrorMessage[InfoLogLength];
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, 0, &FragmentShaderErrorMessage[0]);
		if (InfoLogLength > 0)
		{
			Log(FragmentShaderErrorMessage);
		}

		Log("Fragment shader checked");

		// Link the program
		Log("Linking shader");
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

		return Shader(name, ProgramID);
	}
}
