///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once

typedef unsigned int FrameBufferHandle;
typedef unsigned int ColorBufferHandle;
typedef unsigned int DepthBufferHandle;

#include "pyxieResource.h"
namespace pyxie
{
	class pyxieTexture;
		
	enum RenderBufferQuarity
	{
		QuarityLow,
		QuarityHigh
	};

	class PYXIE_EXPORT pyxieRenderTarget : public pyxieResource
	{
		FrameBufferHandle frameBufferHandle;
		ColorBufferHandle colorBufferHandle;
		DepthBufferHandle depthBufferHandle;
		DepthBufferHandle stencilBufferHandle;
		pyxieTexture* colorTexture;
		pyxieTexture* depthTexture;
		uint32_t	width;
		uint32_t	height;
		bool useStencilBuffer;
		bool useDepthBuffer;
		//bool useColorAlpha;
		float scissorX;
		float scissorY;
		float scissorW;
		float scissorH;
	public:

		///useColor         カラーバッファを使う
		///useDepth         デプスバッファを使う
		///useStencil       ステンシルバッファを使う
		//pyxieRenderTarget(uint32_t w, uint32_t h, bool useColor, bool useDepth, bool useStencil = false, bool useColorAlpha = false);
		pyxieRenderTarget(pyxieTexture* colorTex, bool useDepth, bool useStencil = false);
		pyxieRenderTarget(pyxieRenderTarget* org);
		~pyxieRenderTarget();

		void Build();
		void Initialize();
		void Clone(bool afterFinishBuild);
		RESOURCETYPE ResourceType() { return RENDERTARGETTYPE; }

		void Render();

		///カラーテクスチャを取得する（useColor=trueの時だけ有効）
		inline pyxieTexture* GetColorTexture(){
			return colorTexture;
		}

		///デプステクスチャを取得する（useDepth=true、useDepthTexture=true、useStencil=falseの時だけ有効）
		inline pyxieTexture* GetDepthTexture(){
			return depthTexture;
		}

		///ピクセルイメージをリードする
		bool ReadColorBufferImage(unsigned char* ptr);

		inline uint32_t GetWidth() { return width; }
		inline uint32_t GetHeight() { return height; }

		inline FrameBufferHandle GetFrameBufferHandle(){
			return frameBufferHandle;
		}

		///ビューポートのシザリングエリアを設定する
		///左下(0,0)
		///幅、高さ(1,1)
		inline void SetScissorArea(float x, float y, float w, float h){
			scissorX = x;
			scissorY = y;
			scissorW = w;
			scissorH = h;
		}

#if defined __ENABLE_SUSPEND_RECOVER__
		virtual bool Restore();
		virtual bool Release();
#endif
	};
}