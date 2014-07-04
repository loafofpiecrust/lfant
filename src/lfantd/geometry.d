module lfant.geometry;

import std.container;
import std.stdio;
import std.array;
import std.algorithm;
import unstd.memory.weakref;

import gl3n.linalg;

import derelict.opengl3.gl3;
import derelict.assimp3.assimp;

import lfant.framebuffer;

struct Buffer(T)
{
	void create(int target, int mode = GL_STATIC_DRAW)
	{
		this.target = target;
		glGenBuffers(1, &id);
		bind();
		bufferData(mode);
	}
	
	void destroy()
	{
		glDeleteBuffers(1, &id);
	}

	void bind()
	{
		glBindBuffer(target, id);
	}
	
	static void unbind(int target)
	{
		glBindBuffer(target, 0);
	}
	
	void bufferData(int mode = GL_STATIC_DRAW)
	{
		glBufferData(target, T.sizeof*this.length, &(data[0]), mode);
	}
	
	void insert(in T inst)
	{
		data ~= inst;
	}

	int target = -1;
	uint id = 0;
	
	private T[] data;
	alias data this;
}

class Geometry
{
	static Geometry loadFile(string path)
	{
		if(path.empty()) return null;
	
		for(uint i = 0; i < cache.length; ++i)
		{
			if(!cache[i].alive)
			{
				cache.remove(i);
				--i;
				continue;
			}
			
			auto mesh = cache[i].lock();
			if(mesh.file == path)
			{
				return mesh;
			}
		}
	
		Geometry m = new Geometry;
		cache ~= m.weakReference;
	
		m.file = path;
		m.scene = aiImportFile(path.ptr, aiProcessPreset_TargetRealtime_MaxQuality);
		auto scene = m.scene;
	
		m.posBuffer.clear();
		m.uvBuffer.clear();
		m.normalBuffer.clear();
		for(uint i = 0; i < scene.mNumMeshes; ++i)
		{
			const aiMesh* mesh = scene.mMeshes[i];
			m.posBuffer.reserve(m.posBuffer.length + mesh.mNumVertices);
			m.uvBuffer.reserve(m.uvBuffer.length + mesh.mNumVertices);
			m.normalBuffer.reserve(m.normalBuffer.length + mesh.mNumVertices);
		//	indexBuffer.reserve(indexBuffer.size()+mesh.mNumFaces*3);
	
			for(uint k = 0; k < mesh.mNumVertices; ++k)
			{
				m.posBuffer.insert(cast(vec3)(mesh.mVertices[k]));
				if(mesh.mTextureCoords[0] && &mesh.mTextureCoords[0][k])
				{
					m.uvBuffer.insert((cast(vec3)mesh.mTextureCoords[0][k]).xy);
				}
				m.normalBuffer.insert(cast(vec3)(mesh.mNormals[k]));
			}
			for(uint k = 0; k < mesh.mNumFaces; ++k)
			{
				for(uint j = 0; j < mesh.mFaces[k].mNumIndices; ++j)
				{
					m.indexBuffer.insert(mesh.mFaces[k].mIndices[j]);
				}
			}
		}
	
		aiReleaseImport(scene);
		m.initialize();
		return m;
	}
	
	~this()
	{
		deinit();
	}
	
	void initialize()
	{
		if(posBuffer.empty)
		{
			writeln("Mesh init rejected (no data)");
			return;
		}
	
		if(loaded)
		{
			writeln("Mesh init rejected (previously loaded)");
			return;
		}
	
		FrameBuffer fbo = FrameBuffer.current;
		if(fbo && !fboQuad)
		{
			fbo.unbind();
		}
	
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);
	
		posBuffer.create(GL_ARRAY_BUFFER);
		uvBuffer.create(GL_ARRAY_BUFFER);
		normalBuffer.create(GL_ARRAY_BUFFER);
		indexBuffer.create(GL_ELEMENT_ARRAY_BUFFER);
	
		glEnableVertexAttribArray(0);
		posBuffer.bind();
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, null);
	
		glEnableVertexAttribArray(1);
		uvBuffer.bind();
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, null);
	
		glEnableVertexAttribArray(2);
		normalBuffer.bind();
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, null);
	
		indexBuffer.bind();
	
		glBindVertexArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
		if(fbo && !fboQuad)
		{
			fbo.bind();
		}
	
		loaded = true;
	}
	
	void render()
	{
		if(!loaded)
		{
			return;
		}
	
		glBindVertexArray(vertexArray);
		glDrawElements(GL_TRIANGLES, cast(int)indexBuffer.length, GL_UNSIGNED_INT, null);
		glBindVertexArray(0);
	}

	void deinit()
	{
		posBuffer.destroy();
		uvBuffer.destroy();
		normalBuffer.destroy();
		indexBuffer.destroy();
		loaded = false;
	}
	
private:
	static WeakReference!(Geometry)[] cache;

	string file;
	uint vertexArray;
	bool loaded = false;
	bool fboQuad = false;
	
	Buffer!vec2 uvBuffer;
	Buffer!vec3 posBuffer;
	Buffer!vec3 normalBuffer;
	Buffer!uint indexBuffer;
	
	const(aiScene)* scene;
}


import lfant.component;
import lfant.renderer;

class Mesh : Component
{
	
	override void initialize()
	{
		if(geometry is null)
		{
			game.log("Mesh::Init(): No mesh. Disabling");
		//	Enable(false);
			return;
		}
	
		if(material.shader is null || material.texture is null)
		{
		//	material.loadFile(game.getAssetPath("materials/Diffuse.prop"));
		}
	
	//	material.shader.Bind();
	
		material.shader.addUniform("M");
		material.shader.addUniform("V");
		material.shader.addUniform("P");
		material.shader.addUniform("textureSampler");
		material.shader.addUniform("tiling");
	}
	
	override void render()
	{
		if(geometry is null)
		{
			return;
		}
	
	//	glBindVertexArray(mesh.vertexArray);
		material.shader.bind();
	
	/*	if(owner && usingCamera)
		{
			material.shader.setUniform("P", game.scene.mainCamera.projection);
			material.shader.setUniform("V", game.scene.mainCamera.view);
			material.shader.setUniform("M", owner.transform.matrix);
		}
	
		if(material.texture)
		{
			material.shader.setUniform("tiling", material.tiling);
			material.shader.setUniform("textureSampler", material.texture);
		}*/
	
		geometry.render();
	}
	
	override void deinit()
	{
		
	}
	
private:
	Geometry geometry;
	Material material;
	
	bool usingCamera = true;
}