/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
 *	Created: 2013-01-15 by Taylor Snead
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
#include <lfant/Texture.hpp>

// External
#include <GL/glew.h>
#include <lfant/lodepng.hpp>

// Internal
#include <lfant/TextureLoader.hpp>
#include <lfant/String.hpp>

#include <lfant/Console.hpp>

namespace lfant
{

void Texture::LoadFile(string path, int mode)
{
	if(mode == 0)
	{
		mode = GL_TEXTURE_2D;
	}
	Log("Want to load an image file");
	name = path;
	vector<string> tokens = Split(name, ".", "");
	string ext = tokens[tokens.size()-1];
	to_lower(ext);
	Log("Loading an image file of type " + ext);
	vector<byte> data;
	if (ext == "bmp")
	{
		Log("Texture::LoadFile: Unsupported image format, bitch.");
		//	return LoadBMP(name, buffer);
	}
	else if (ext == "jpeg" || ext == "jpg")
	{
		LoadJPEG(mode);
	}
	else if (ext == "png")
	{
		Log("About to load a png");
		LoadPNG(mode);
	}
	else if (ext == "dds")
	{
		LoadDDS(mode);
	}
	else
	{
		Log("Texture::LoadFile: Unsupported image format, bitch.");
		return;
	}


	// Send texture to opengl

}

void Texture::LoadPNG(int mode)
{
	vector<byte> data;
	uint error = lodepng::decode(data, width, height, name);
	if(error != 0)
	{
		Log("LoadPNG: Error, ", lodepng_error_text(error));
		return;
	}
	Log("LoadPNG: Image size: ", width, "x", height);
	if(id == 0)
	{
		glGenTextures(1, &this->id);
		Log("Texture::LoadPNG: GL Texture generated");
	}
	glBindTexture(mode, this->id);
	Log("Texture::LoadPNG: GL Texture bound");
	glTexImage2D(mode, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
	Log("Texture::LoadPNG: texture data sent");

	glTexParameteri(mode, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(mode, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	Log("Texture::LoadPNG: texture filtering set");

	glBindTexture(mode, 0);
}

void Texture::LoadJPEG(int mode)
{
}

void Texture::LoadBMP(int mode)
{
	vector<byte> data;
	byte header[54];
	uint dataPos;
//	uint width, height;
	uint imageSize;
//	vector<byte> data;

	FILE* file = fopen(name.c_str(), "rb");
	if (!file)
	{
		Log("Image " + name + " could not be loaded");
		return;
	}

	if (fread(header, 1, 54, file) != 54 || (header[0] != 'B' || header[1] != 'M'))
	{
		Log("Image " + name + " is not a correct BMP file");
		return;
	}

	// Grab header data
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	Log("Read image header");

	// Make up any missing header data
	if (imageSize == 0) imageSize = width * height * 3;
	if (dataPos == 0) dataPos = 54;

	// Buffer
	data.reserve(imageSize);
	// Read data from file to buffer
	fread(&data[0], 1, imageSize, file);
	// Close file
	fclose(file);

	Log("Read data and closed file");
	if(id == 0)
	{
		glGenTextures(1, &this->id);
	}
	glBindTexture(mode, this->id);
	glTexImage2D(mode, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);

	glTexParameteri(mode, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(mode, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(mode, 0);
}

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

void Texture::LoadDDS(int mode)
{
	unsigned char header[124];

	FILE *fp;

	/* try to open the file */
	fp = fopen(name.c_str(), "rb");
	if (fp == NULL)
		return;

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	height      = *(unsigned int*)&(header[8 ]);
	width	     = *(unsigned int*)&(header[12]);
	unsigned int linearSize	 = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC      = *(unsigned int*)&(header[80]);


	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch(fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return;
	}

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(mode, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize;
		glCompressedTexImage2D(mode, level, format, width, height,
			0, size, buffer + offset);

		offset += size;
		width  /= 2;
		height /= 2;
	}

	free(buffer);
	id = textureID;
	Log("Finished loading DDS texture");
}

}
