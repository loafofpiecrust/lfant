module lfant.renderer;

import std.typecons;
import std.array;
import std.container;
import std.stream;
import std.algorithm;
import std.stdio;

import unstd.memory.weakref;

import derelict.opengl3.gl3;

import lfant.subsystem;
import lfant.game;
import lfant.framebuffer;
import lfant.properties;
import lfant.transform;

class Renderer : Subsystem
{
	this(Game g)
	{
		super(g);
	}
	
	~this()
	{
		writeln("renderer");
	}
	
	override void initialize()
	{
		loadFile("settings/renderer.prop");
		
	//	glShadeModel(GL_SMOOTH);
	//	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
		// Background color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);
	
		// Backface culling
	//	glEnable(GL_CULL_FACE);
	//	glFrontFace(GL_CCW);
	
		// Texture and shading
		glEnable(GL_TEXTURE_2D);
	
		game.log("Renderer: Initialized");
	
		frameBuffer = new FrameBuffer(game);
		frameBuffer.addTexture("diffuseTex", Texture.Format.RGBA, Texture.Format.RGBA);
		frameBuffer.addTexture("positionTex", Texture.Format.RGB32F, Texture.Format.RGB);
		frameBuffer.addTexture("normalTex", Texture.Format.RGB32F, Texture.Format.RGB);
		frameBuffer.addTexture("lightTex", Texture.Format.RGBA, Texture.Format.RGBA);
	//	frameBuffer.AddTexture("specularTex", Texture.Format.RGBA, Texture.Format.RGBA);
		frameBuffer.addDepthTexture("depthTex");
	
		vec2i res = game.window.size;
		game.log("Making framebuffer of size ", res);
		frameBuffer.rect = URect(0,0, res.x, res.y);
		frameBuffer.initialize();
	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	override void render()
	{
		game.window.render();
		frameBuffer.bind();
		frameBuffer.drawBuffers();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//	glDepthMask(true);
		glEnable(GL_DEPTH_TEST);
	
		static bool physicsSwitch = false;
		if(game.input.getButtonDown("SwitchRendering") == 1)
		{
			physicsSwitch = !physicsSwitch;
			std.stdio.writeln("switch");
		}
	
		if(physicsSwitch)
		{
		//	game.physics.Render();
		}
		else
		{
			game.scene.render();
		}
	
	
	/*	if(lights.size() > 0)
		{
			glDrawBuffer(GL_NONE);
	
			//
			// stencil pass
			//
			glEnable(GL_STENCIL_TEST);
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			glClear(GL_STENCIL_BUFFER_BIT);
	
			glStencilFunc(GL_ALWAYS, 0, 0);
			glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
			glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
	
			foreach(l; lights)
			{
				l.RenderStencil();
			}
	
			//
			// light pass
			//
			frameBuffer.drawBuffer(3);
			frameBuffer.bindTextures();
	
			glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
	
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_ONE, GL_ONE);
	
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
	
			foreach(l; lights)
			{
				l.Render();
			}
	
			glCullFace(GL_BACK);
			glDisable(GL_BLEND);
			glDisable(GL_STENCIL_TEST);
		}*/
	//	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
	
		glDisable(GL_DEPTH_TEST);
	//	glEnable(GL_BLEND);
	//	glBlendEquation(GL_FUNC_ADD);
	//	glBlendFunc(GL_ONE, GL_ONE);
	
	//	glEnable(GL_CULL_FACE);
	//	glCullFace(GL_FRONT);
	//	}
	
	//	resize(res);
	
		//
		// final pass
		//
		frameBuffer.unbind();
		frameBuffer.render();
	}
	
	void resize(vec2i res)
	{
		glViewport(0, 0, res.x, res.y);
	}
	
	FrameBuffer frameBuffer;
	
private:
	float ambientLight = 0.1f;
}

class Shader
{
	~this()
	{
		glDeleteProgram(id);
	}
	
	static Shader loadFile(string vert, string frag, string geom, string comp)
	{
		for(uint i = 0; i < cache.length; ++i)
		{
			if(!cache[i].alive)
			{
				cache.remove(i);
			}
			
			auto sh = cache[i].lock();
			if(sh.vertex == vert && sh.fragment == frag)
			{
				return sh;
			}
		}
	
		Shader sh = new Shader;
		cache ~= sh.weakReference;
	
		sh.vertex = vert;
		sh.fragment = frag;
		sh.geometry = geom;
		sh.compute = comp;
	
		sh.compile();
	
		return sh;
	}

	void serialize(Properties prop)
	{
		prop.value("vertex", vertex);
		prop.value("fragment", fragment);
		prop.value("geometry", geometry);
	}
	
	void compile()
	{
		if(vertex.empty() || fragment.empty())
		{
			return;
		}
	
		uint vert = -1;
		uint frag = -1;
		id = glCreateProgram();
	
		if(!vertex.empty())
		{
			vert = Shader.compile(GL_VERTEX_SHADER, vertex);
			glAttachShader(id, vert);
		}
		if(!fragment.empty())
		{
			frag = Shader.compile(GL_FRAGMENT_SHADER, fragment);
			glAttachShader(id, frag);
		}
	
//	#if !LFANT_GLES
		uint geom = -1;
		uint comp = -1;
		if(!geometry.empty())
		{
			geom = Shader.compile(GL_GEOMETRY_SHADER, geometry);
			glAttachShader(id, geom);
		}
		if(!compute.empty())
		{
			comp = Shader.compile(GL_COMPUTE_SHADER, compute);
			glAttachShader(id, comp);
		}
//	#endif
	
		glLinkProgram(id);
	//	CheckErrors();
	
		if(vert != -1) glDeleteShader(vert);
		if(frag != -1) glDeleteShader(frag);
//	#if !LFANT_GLES
		if(geom != -1) glDeleteShader(geom);
		if(comp != -1) glDeleteShader(comp);
//	#endif
	
		unbind();
	}
	
	void bind()
	{
		if(current !is this)
		{
			glUseProgram(id);
			current = this;
		}
	}
	
	void unbind() const
	{
		if(current !is null)
		{
			glUseProgram(0);
			current = null;
		}
	}
	
	uint getUniform(string name)
	{
		return uniforms[name];
	}
	
	void addUniform(string name)
	{
		uniforms[name] = glGetUniformLocation(id, name.ptr);
	}
	
	void setUniform(string name, in float val)
	{
		glUniform1f(getUniform(name), val);
	}
	
	void setUniform(string name, in int val)
	{
		glUniform1i(getUniform(name), val);
	}
	
	void setUniform(string name, in uint val)
	{
		glUniform1ui(getUniform(name), val);
	}
	
	void setUniform(string name, in vec2 val)
	{
		glUniform2f(getUniform(name), val.x, val.y);
	}
	
	void setUniform(string name, in vec3 val)
	{
		glUniform3f(getUniform(name), val.x, val.y, val.z);
	}
	
	void setUniform(string name, in vec4 val)
	{
		glUniform4f(getUniform(name), val.x, val.y, val.z, val.w);
	}
	
	void setUniform(string name, in mat4 val)
	{
		glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, &val[0][0]);
	}
	
	void setUniform(string name, Texture val)
	{
		val.bind();
		glUniform1i(getUniform(name), val.index);
	//	val->Unbind();
	}
	
private:
	
	static uint compile(uint type, in string path)
	{
		auto stream = new std.stream.File(path, FileMode.In);
		string line = "";
		string source = "";
	
		string[] str;
		while(!stream.eof())
		{
			line = cast(string)stream.readLine();
			// extra parsing code?
		/*	if(line[0] == '#')
			{
				str = Split(line, "\t <>\"");
				if(str[0] == "#include")
				{
					string p = path;
					p = p.remove_filename()~"/"~str[1];
					auto stream2 = new File(p, FileMode.In);
					string line2 = "";
					while(!stream2.eof())
					{
						line2 = cast(string)stream2.readLine();
						source ~= "\n"~line2;
					}
					stream2.close();
					continue;
				}
			}*/
			source ~= "\n"~line;
		}
		stream.close();
	
		uint shader = glCreateShader(type);
		const(char*) csource = (source~"\0").ptr;
		glShaderSource(shader, 1, &csource, null);
		glCompileShader(shader);
	
		int logLength = 0;
		int result = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		char[] error = new char[logLength];
		glGetShaderInfoLog(shader, logLength, null, &error[0]);
		if (logLength > 0)
		{
			std.stdio.writeln(error);
		}
	
		return shader;
	}

	static WeakReference!(Shader)[] cache;
	static Shader current;
	
	string vertex;
	string fragment;
	string geometry;
	string compute;
	
	uint id = -1;
	uint[string] uniforms;
}


class Texture
{
	enum Format : uint
	{
		RGB = GL_RGB,
		RGBA = GL_RGBA,
		RGBA16F = GL_RGBA16F,
		RGB32F = GL_RGB32F,
		RGBA32F = GL_RGBA32F,

		CompressedRGB = GL_COMPRESSED_RGB,
		CompressedRGBA = GL_COMPRESSED_RGBA,

		Depth = GL_DEPTH_COMPONENT,
		DepthStencil = GL_DEPTH_STENCIL,
		Depth24 = GL_DEPTH_COMPONENT24,
		Depth32F = GL_DEPTH_COMPONENT32F,

		Depth32F_Stencil8 = GL_DEPTH32F_STENCIL8,
		Depth24_Stencil8 = GL_DEPTH24_STENCIL8
	}
	enum WrapMode : uint
	{
		Clamp = GL_CLAMP_TO_EDGE,
		Repeat = GL_REPEAT
	}
	enum ScaleFilter : uint
	{
		Nearest = GL_NEAREST,
		Linear = GL_LINEAR
	}
	enum DataType : uint
	{
		Byte = GL_UNSIGNED_BYTE,
		Float = GL_FLOAT,
		Uint24_8 = GL_UNSIGNED_INT_24_8,
	}
	
	static Texture loadFile(string path, int mode = GL_TEXTURE_2D)
	{
		if(path.empty) return null;
	
		for(uint i = 0; i < cache.length; ++i)
		{
			if(!cache[i].alive)
			{
				cache.remove(i);
				--i;
				continue;
			}
			
			auto tex = cache[i].lock();
			if(tex.path == path && tex.mode == mode)
			{
				return tex;
			}
		}
	
		Texture tex = new Texture;
		cache ~= tex.weakReference;
	
		tex.mode = mode;
		/// @todo Implement png loading
	//	tex.LoadPNG(path);
		return tex;
	}
	static Texture load(Properties prop)
	{
		return null;
	}
	
	void serialize(Properties prop)
	{
		prop.value("file", path);
	}
	
	void initData(const(ubyte*) data)
	{
		glGenTextures(1, &_id);
		bind();
	
		if(mode == GL_TEXTURE_2D_MULTISAMPLE)
		{
			glTexImage2DMultisample(mode, msaa, internalFormat, size.x, size.y, false);
		}
		else
		{
			glTexImage2D(mode, 0, internalFormat, size.x, size.y, 0, format, dataType, data);
		}
	
		glTexParameteri(mode, GL_TEXTURE_MAG_FILTER, scaleFilter);
		glTexParameteri(mode, GL_TEXTURE_MIN_FILTER, scaleFilter);
	
		glTexParameteri(mode, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(mode, GL_TEXTURE_WRAP_T, wrapMode);
	
		unbind();
	}
	
	void bind()
	{
		if(current !is this)
		{
			glActiveTexture(GL_TEXTURE0 + index);
			glBindTexture(mode, id);
			current = this;
		}
	}
	
	void unbind() const
	{
		if(current !is null)
		{
			glActiveTexture(GL_TEXTURE0 + index);
			glBindTexture(mode, 0);
			current = null;
		}
	}
	
	void setFormats(Format f1, Format f2)
	{
		internalFormat = f1;
		format = f2;
	}
	
	@property
	{
		int mode() const
		{
			return _mode;
		}
		private void mode(in int m)
		{
			_mode = m;
		}
		
		uint id() const
		{
			return _id;
		}
		private void id(in uint i)
		{
			_id = i;
		}
	}
	
	string path;
	
	vec2u size;
	int index = 0;
	ubyte anisoLevel = 1;
	ubyte msaa = 0;
	
	DataType dataType = DataType.Byte;
	Format internalFormat = Format.RGBA;
	Format format = Format.RGBA;
	WrapMode wrapMode = WrapMode.Clamp;
	ScaleFilter scaleFilter = ScaleFilter.Nearest;
	
private:
	static WeakReference!(Texture)[] cache;
	static Texture current;

	uint _id = 0;
	int _mode = GL_TEXTURE_2D;
}

//static Array!(Texture) textureCache;

struct Material
{
	Texture texture;
	Shader shader;
}

import gl3n.math;
import gl3n.frustum;
import gl3n.util;
import gl3n.interpolate;
import gl3n.linalg;
import lfant.component;

class Camera : Component
{
public:
	enum Mode : ubyte
	{
		Perspective,
		Orthographic
	}

	override void initialize()
	{
		updateProjection();
	/*	if(game.scene.mainCamera is null)
		{
			game.scene.mainCamera = this;
		}*/
	}

	override void update(in float delta)
	{
		updateView();
	}

	override void serialize(Properties prop)
	{
		prop.value("mode", _mode);
		prop.value("fov", _fov);
	}

	final @property
	{
		float fov() const
		{
			return _fov;
		}
		void fov(in float f)
		{
			_fov = f;
			updateProjection();
		}

		float aspectRatio() const
		{
			return _aspectRatio;
		}
		void aspectRatio(in float ar)
		{
			_aspectRatio = ar;
			updateProjection();
		}

		float aperture() const
		{
			return _aperture;
		}
		void aperture(in float ar)
		{
			_aperture = ar;
			updateProjection();
		}

		Mode mode() const
		{
			return _mode;
		}
		void mode(in Mode m)
		{
			_mode = m;
			updateProjection();
		}

		vec2 viewRange() const
		{
			return _viewRange;
		}
		void viewRange(in vec2 ar)
		{
			_viewRange = ar;
			updateProjection();
		}
	}

private:
	void updateProjection()
	{
		switch(mode)
		{
			case Mode.Perspective:
			{
			//	projection = mat4.perspective(fov/aperture, aspectRatio, viewRange.x, viewRange.y);
				projection = mat4.perspective(16.0f, 9.0f, fov/aperture, viewRange.x, viewRange.y);
				//	GetGame()->Log("projection: ", projection);
				break;
			}
			case Mode.Orthographic:
			{
				float w = (fov/aperture * aspectRatio)*0.5f;
				float h = (fov/aperture / aspectRatio)*0.5f;
				projection = mat4.orthographic(w, -w, h, -h, viewRange.x, viewRange.y);
				break;
			}
			default: break;
		}
	}

	void updateView()
	{
		vec3 pos = owner.transform.worldPositionRelative;
		view = mat4.look_at(
			pos,
			pos + owner.transform.direction,
			owner.transform.up
			);

	}

	mat4 projection;
	mat4 view;

	Mode _mode;
	float _aspectRatio = 16.0f/9.0f;
	float _fov = 90.0f;
	float _aperture = 1.0f;
	vec2 _viewRange = vec2(0.01f, 1000.0f);
}