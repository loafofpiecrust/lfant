
#include "Renderer.h"

#include "lfant/Renderer.h"
#include "lfant/Game.h"
#include "lfant/Console.h"
#include <lfant/Scene.h>
#include <lfant/Camera.h>
#include "lfant/Window.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Rocket/Core.h>
#include <GL/glew.h>

namespace lfant {
namespace gui {
namespace rocket {

Renderer::Renderer()
{
}

void Renderer::SetViewport(int width, int height)
{
	viewport = {width, height};
}

/// @todo Improve GUI rendering process
Rocket::Core::Vertex* curr_verts = nullptr;
int* curr_inds = nullptr;
void Renderer::RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation)
{
	if(texture != mesh.material->texture->GetId())
	{
	//	Log("new texture id ", texture);
		mesh.material->texture->mode = GL_TEXTURE_2D;
		mesh.material->texture->id = texture;
	}

//	if(mesh.vertexBuffer.size() != num_vertices || mesh.indexBuffer.size() != num_indices || (inited && mesh.uvBuffer[0] != vec2_cast<vec2>(vertices[0].tex_coord)))
	{
	//	curr_verts = vertices;
	//	curr_inds = indices;

		mesh.vertexBuffer.data.resize(num_vertices);
		mesh.uvBuffer.data.resize(num_vertices);
		colorBuffer.data.resize(num_vertices);
		for(int i = 0; i < num_vertices; ++i)
		{
			mesh.vertexBuffer[i] = vec3(vec2_cast<vec2>(vertices[i].position), 0.0f);
			mesh.uvBuffer[i] = vec2_cast<vec2>(vertices[i].tex_coord);
			colorBuffer[i] = vec3_cast<vec3>(vertices[i].colour);
		}

		mesh.indexBuffer.data.resize(num_indices);
		for(int i = 0; i < num_indices; ++i)
		{
			mesh.indexBuffer[i] = indices[i];
		//	Log("gui index: ", mesh.indexBuffer[i]);
		}

	//	Camera* cam = game->scene->mainCamera;
	//	Log()
	//	changed = true;
	}

	if(!inited)
	{
		mesh.material->shader->LoadFile("shaders/UserInterface.vert", "shaders/UserInterface.frag");
		mesh.usingCamera = false;
		mesh.material->shader->AddUniform("translation");
		mesh.material->shader->AddUniform("resolution");
		mesh.material->shader->AddUniform("projection");
		mesh.Init();
		inited = true;

		vec2 res = (vec2)game->window->GetSize();
		projection = glm::ortho(0.0f, res.x, res.y, 0.0f, 0.0f, 3.0f);
	}
//	else if(changed)
	{
	//	Log("changed. buffering new data");
		Mesh::BufferData(mesh.vertexBuffer);
		Mesh::BufferData(mesh.uvBuffer);
		Mesh::BufferData(mesh.indexBuffer);
	//	changed = false;
	}

	// Actual rendering
	mesh.BeginRender();

	mesh.material->shader->SetUniform("translation", vec2_cast<vec2>(translation));
	mesh.material->shader->SetUniform("resolution", (vec2)game->window->GetSize());
	mesh.material->shader->SetUniform("projection", projection);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	mesh.MidRender();
	mesh.EndRender();

	glDisableVertexAttribArray(2);

//	glPushMatrix();
/*	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ivec2 res = game->renderer->GetResolution();
	glOrtho(0, res.x, res.y, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(translation.x, translation.y, 0);

	glVertexPointer(2, GL_FLOAT, sizeof(Rocket::Core::Vertex), &vertices[0].position);
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Rocket::Core::Vertex), &vertices[0].colour);

	if (!texture)
	{
		glDisable(GL_TEXTURE_2D);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	else
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, (GLuint) texture);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(Rocket::Core::Vertex), &vertices[0].tex_coord);
	}

	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, indices);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	*/
}


// Called by Rocket when it wants to compile geometry it believes will be static for the forseeable future.
Rocket::Core::CompiledGeometryHandle Renderer::CompileGeometry(Rocket::Core::Vertex* ROCKET_UNUSED(vertices), int ROCKET_UNUSED(num_vertices), int* ROCKET_UNUSED(indices), int ROCKET_UNUSED(num_indices), const Rocket::Core::TextureHandle ROCKET_UNUSED(texture))
{
	return (Rocket::Core::CompiledGeometryHandle) NULL;
}

// Called by Rocket when it wants to render application-compiled geometry.
void Renderer::RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle ROCKET_UNUSED(geometry), const Rocket::Core::Vector2f& ROCKET_UNUSED(translation))
{
}

// Called by Rocket when it wants to release application-compiled geometry.
void Renderer::ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle ROCKET_UNUSED(geometry))
{
}

// Called by Rocket when it wants to enable or disable scissoring to clip content.
void Renderer::EnableScissorRegion(bool enable)
{
	if (enable)
		glEnable(GL_SCISSOR_TEST);
	else
		glDisable(GL_SCISSOR_TEST);
}

// Called by Rocket when it wants to change the scissor region.
void Renderer::SetScissorRegion(int x, int y, int width, int height)
{
	glScissor(x, viewport.y - (y + height), width, height);
}

// Set to byte packing, or the compiler will expand our struct, which means it won't read correctly from file
#pragma pack(1)
struct TGAHeader
{
	char  idLength;
	char  colourMapType;
	char  dataType;
	short int colourMapOrigin;
	short int colourMapLength;
	char  colourMapDepth;
	short int xOrigin;
	short int yOrigin;
	short int width;
	short int height;
	char  bitsPerPixel;
	char  imageDescriptor;
};
// Restore packing
#pragma pack()
// Called by Rocket when a texture is required by the library.
bool Renderer::LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source)
{
	Rocket::Core::FileInterface* file_interface = Rocket::Core::GetFileInterface();
	Rocket::Core::FileHandle file_handle = file_interface->Open(source);
	if (!file_handle)
	{
		return false;
	}

	file_interface->Seek(file_handle, 0, SEEK_END);
	std::size_t buffer_size = file_interface->Tell(file_handle);
	file_interface->Seek(file_handle, 0, SEEK_SET);

	char* buffer = new char[buffer_size];
	file_interface->Read(buffer, buffer_size, file_handle);
	file_interface->Close(file_handle);

	TGAHeader header;
	memcpy(&header, buffer, sizeof(TGAHeader));

	int color_mode = header.bitsPerPixel / 8;
	int image_size = header.width * header.height * 4; // We always make 32bit textures

	if (header.dataType != 2)
	{
		Rocket::Core::Log::Message(Rocket::Core::Log::LT_ERROR, "Only 24/32bit uncompressed TGAs are supported.");
		return false;
	}

	// Ensure we have at least 3 colors
	if (color_mode < 3)
	{
		Rocket::Core::Log::Message(Rocket::Core::Log::LT_ERROR, "Only 24 and 32bit textures are supported");
		return false;
	}

	const char* image_src = buffer + sizeof(TGAHeader);
	unsigned char* image_dest = new unsigned char[image_size];

	// Targa is BGR, swap to RGB and flip Y axis
	for (long y = 0; y < header.height; y++)
	{
		long read_index = y * header.width * color_mode;
		long write_index = ((header.imageDescriptor & 32) != 0) ? read_index : (header.height - y - 1) * header.width * color_mode;
		for (long x = 0; x < header.width; x++)
		{
			image_dest[write_index] = image_src[read_index+2];
			image_dest[write_index+1] = image_src[read_index+1];
			image_dest[write_index+2] = image_src[read_index];
			if (color_mode == 4)
				image_dest[write_index+3] = image_src[read_index+3];
			else
				image_dest[write_index+3] = 255;

			write_index += 4;
			read_index += color_mode;
		}
	}

	texture_dimensions.x = header.width;
	texture_dimensions.y = header.height;

	bool success = GenerateTexture(texture_handle, image_dest, texture_dimensions);

	delete [] image_dest;
	delete [] buffer;

	return success;
}

// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
bool Renderer::GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const Rocket::Core::byte* source, const Rocket::Core::Vector2i& source_dimensions)
{
	GLuint texture_id = 0;
	glGenTextures(1, &texture_id);
	if (texture_id == 0)
	{
		printf("Failed to generate textures\n");
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, source_dimensions.x, source_dimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, source);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	texture_handle = (Rocket::Core::TextureHandle) texture_id;

	return true;
}

// Called by Rocket when a loaded texture is no longer required.
void Renderer::ReleaseTexture(Rocket::Core::TextureHandle texture_handle)
{
	glDeleteTextures(1, (GLuint*) &texture_handle);
}

} // rocket
} // gui
} // lfant
