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

#include "TextureLoader.hpp"

#define PNG_STDIO_SUPPORTED 1

// External
#include <GL/glew.h>
#include <boost/algorithm/string.hpp>
#include <png.h>
#include <pngconf.h>
#include <pnginfo.h>
#include <setjmp.h>

// Internal
#include "Console.hpp"
#include "StringUtil.hpp"

namespace sfs
{

	Texture LoadTexture( string name )
	{
		Log("Want to load an image file");
		string ext = split(name, ".", "")[1];
		boost::to_upper(ext);
		Log("Loading an image file of type " + ext);
		if (ext == "BMP")
		{
			return LoadBMP(name);
		}
		else if (ext == "JPEG" || ext == "JPG")
		{
			return LoadJPEG(name);
		}
		else if (ext == "PNG")
		{
			Log("About to load a png");
			return LoadPNG(name);
		}
	}

	Texture LoadBMP( string name )
	{
		byte header[54];
		uint dataPos;
		uint width, height;
		uint imageSize;
		byte* data;

		FILE* file = fopen(name.c_str(), "rb");
		if (!file)
		{
			Log("Image " + name + " could not be loaded");
			return null(Texture) ;
		}

		if (fread(header, 1, 54, file) != 54 || (header[0] != 'B' || header[1] != 'M'))
		{
			Log("Image " + name + " is not a correct BMP file");
			return null(Texture) ;
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
		data = new byte[imageSize];
		// Read data from file to buffer
		fread(data, 1, imageSize, file);
		// Close file
		fclose(file);

		Log("Read data and closed file");

		// Bind to OpenGL
		uint32_t id = 0;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

		Log("Generated image in OpenGL");

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		Log("Set some tex parameters");

		glBindTexture(GL_TEXTURE_2D, 0);

		return Texture(name, width, height, id);
	}

	Texture LoadJPEG( string name )
	{

	}

	Texture LoadPNG( string name )
	{
		png_structp png_ptr;
		png_infop info_ptr;
		uint sig_read = 0;
		int color_type, interlace_type;
		FILE* file;

		if ((file = fopen(name.c_str(), "rb")) == 0)
		{
			Log("Texture: Couldn't find "+name);
			return null(Texture) ;
		}

		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
		if (png_ptr == 0)
		{
			fclose(file);
			Log("Texture: Couldn't load "+name);
			return null(Texture) ;
		}

		info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr == 0)
		{
			fclose(file);
			png_destroy_read_struct(&png_ptr, (png_infopp)(0), (png_infopp)(0));
			Log("Texture: Couldn't load "+name);
			return null(Texture) ;
		}

		if (setjmp(png_jmpbuf(png_ptr)))
		{
			png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)(0));
			fclose(file);
			Log("Texture: Couldn't load "+name);
			return null(Texture) ;
		}

		png_init_io(png_ptr, file);
		png_set_sig_bytes(png_ptr, sig_read);

		// Read the file
		png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, (png_voidp)0);

		uint16_t width, height;
		width = info_ptr->width;
		height = info_ptr->height;

		bool hasAlpha;
		switch (info_ptr->color_type)
		{
			case PNG_COLOR_TYPE_RGBA:
				hasAlpha = true;
				break;
			case PNG_COLOR_TYPE_RGB:
				hasAlpha = false;
				break;
			default:
				Log("Color type ", info_ptr->color_type, " not supported");
				png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
				fclose(file);
				return null(Texture) ;
				break;
		}
		uint row_bytes = png_get_rowbytes(png_ptr, info_ptr);
		byte* data = (byte*)malloc(row_bytes * height);

		png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

		for (uint i = 0; i < height; ++i)
		{
			memcpy(data + (row_bytes * (height - 1 - i)), row_pointers[i], row_bytes);
		}

		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

		fclose(file);

		uint32_t id = 0;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, hasAlpha ? GL_RGBA : GL_RGB, width, height, 0, hasAlpha ? GL_RGBA : GL_RGB,
				GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);

		return Texture(name, width, height, id);
	}

	Texture LoadTGA( string name )
	{
		/// @todo Revise to get image size as well.
		/*
		 // Create one OpenGL texture
		 uint32_t textureID = 0;
		 glGenTextures(1, &textureID);

		 // "Bind" the newly created texture : all future texture functions will modify this texture
		 glBindTexture(GL_TEXTURE_2D, textureID);

		 // Read the file, call glTexImage2D with the right parameters
		 glfwLoadTexture2D(name.c_str(), 0);

		 // Nice trilinear filtering.
		 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		 glGenerateMipmap(GL_TEXTURE_2D);

		 return Texture(name, 0, 0, textureID);*/
	}
}
