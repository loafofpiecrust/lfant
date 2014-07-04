module lfant.framebuffer;

import std.container;

import derelict.opengl3.gl3;

import lfant.gameobject;
import lfant.game;
import lfant.renderer;

public import lfant.rect;
public import gl3n.linalg;

class FrameBuffer : GameObject
{
	this(Game g)
	{
		_game = g;
	}
	
	override void initialize()
	{
		glGenFramebuffers(1, &id);
		glBindFramebuffer(GL_FRAMEBUFFER, id);
		
		attachments.clear();
		Texture tex = null;
		if(textures.length > 0)
		{
			for(uint i = 0; i < textures.length; ++i)
			{
				tex = textures[i];
				tex.wrapMode = Texture.WrapMode.Clamp;
				tex.size = vec2u(rect.w, rect.h);
				tex.index = i;
				tex.initData(null);
				
				tex.bind();
				uint attachment = GL_COLOR_ATTACHMENT0 + i;
				glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, tex.mode, tex.id, 0);
			
				attachments.insert(attachment);
			}
		}
		
			
		if(depthTexture !is null)
		{
		//	depthTexture->SetFormat(Texture::Format::Depth32F_Stencil8, Texture::Format::Depth);
			depthTexture.setFormats(Texture.Format.Depth32F, Texture.Format.Depth);
			depthTexture.wrapMode = Texture.WrapMode.Clamp;
			depthTexture.dataType = Texture.DataType.Float;
			depthTexture.size = vec2u(rect.w, rect.h);
			depthTexture.index = cast(int)textures.length;
			depthTexture.initData(null);

			depthTexture.bind();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depthTexture.mode, depthTexture.id, 0);

			game.log(glGetError());
		}
	}
	
	override void deinit()
	{
	}
	
	void bind()
	{
		
	}
	
	void unbind() const
	{
		
	}
	
	void addTexture(string name, Texture.Format form1, Texture.Format form2)
	{
		Texture tex = new Texture;
		tex.path = name;
		tex.setFormats(form1, form2);
		textures.insert(tex);
	}
	
	void addDepthTexture(string name, Texture.Format form1 = Texture.Format.RGB32F, Texture.Format form2 = Texture.Format.RGB32F)
	{
		if(depthTexture is null)
		{
			depthTexture = new Texture;
		}
		depthTexture.path = name;
	}
	
	void drawBuffers()
	{
		
	}
	
	@property
	{
		static FrameBuffer current()
		{
			return _current;
		}
		
		override Game game()
		{
			return _game;
		}
		
		bool hasDepth() const
		{
			return depthTexture !is null;
		}
	}
	
	URect rect;
	
private:

	uint id = 0;
	uint depthBuffer = 0;
	
	Array!(Texture) textures;
	Array!(uint) attachments;
	
	Texture depthTexture;
	Shader shader;

	static FrameBuffer _current;
	Game _game;
}