/*
	GWEN
	Copyright (c) 2011 Facepunch Studios
	See license in Gwen.h
*/

#pragma once

#include "Gwen/Gwen.h"
#include "Gwen/BaseRender.h"
#include <glm/matrix.hpp>
#include <lfant/Shader.h>
#include <memory>

namespace lfant {
class Window;
}

namespace Gwen
{
	namespace Renderer
	{

		class OpenGL : public Gwen::Renderer::Base
		{
			public:

				struct Vertex
				{
					float x, y;
					float u, v;
				//	unsigned char r, g, b, a;
				};

				OpenGL(lfant::Window* window);
				~OpenGL();

				virtual void Init();

				virtual void Begin();
				virtual void End();

				virtual void SetDrawColor( Gwen::Color color );
				virtual void DrawFilledRect( Gwen::Rect rect );

				virtual void LoadFont(Gwen::Font* font);
				virtual void FreeFont(Gwen::Font* font);
				virtual void RenderText(Gwen::Font* pFont, Gwen::Point pos, const Gwen::UnicodeString& text);

				void StartClip();
				void EndClip();

				void DrawTexturedRect( Gwen::Texture* pTexture, Gwen::Rect pTargetRect, float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f );
				void LoadTexture( Gwen::Texture* pTexture );
				void FreeTexture( Gwen::Texture* pTexture );
				Gwen::Color PixelColour( Gwen::Texture* pTexture, unsigned int x, unsigned int y, const Gwen::Color & col_default );

			protected:

				static const int	MaxVerts = 1024;


				void Flush();
				void AddVert( int x, int y, float u = 0.0f , float v = 0.0f );

				Gwen::Color			m_Color;
			//	int					m_iVertNum;
			//	Vertex				m_Vertices[ MaxVerts ];
				std::vector<Vertex>		m_Vertices;
				unsigned int			m_VertBuffer;


			public:

				//
				// Self Initialization
				//
				virtual bool BeginContext( Gwen::WindowProvider* pWindow );
				virtual bool EndContext( Gwen::WindowProvider* pWindow );

				lfant::Window* window;
				void*	m_pContext;
				mat4 projection;
				mat4 altproj;
				std::shared_ptr<lfant::Shader> shader;
				lfant::Texture* texture = nullptr;
		};

	}
}
