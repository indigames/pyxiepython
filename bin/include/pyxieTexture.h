///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once

#include "pyxieResource.h"
#include "pyxieFigurestruct.h"

namespace pyxie
{
	constexpr uint8_t KtxIdentifier[12] = { 0xAB, 0x4B, 0x54, 0x58, 0x20, 0x31, 0x31, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A };

	typedef unsigned int TextureHandle;

	struct ktxheader {
		char identifier[12];
		uint32_t endianness;
		uint32_t glType;
		uint32_t glTypeSize;
		uint32_t glFormat;
		uint32_t glInternalFormat;
		uint32_t glBaseInternalFormat;
		uint32_t pixelWidth;
		uint32_t pixelHeight;
		uint32_t pixelDepth;
		uint32_t numberOfArrayElements;
		uint32_t numberOfFaces;
		uint32_t numberOfMipmapLevels;
		uint32_t bytesOfKeyValueData;
	};

	class PYXIE_EXPORT pyxieTexture : public pyxieResource{
		TextureHandle		texHandle;
		uint8_t*			image;

		uint32_t			textureType;
		uint32_t			width;
		uint32_t			height;
		uint32_t			color;
		uint32_t			numMips;
		bool				useAlphaChannel;
		SamplerState		currentState;
		bool				fromFile;

	public:

		pyxieTexture(const char* path);
		pyxieTexture(uint32_t w, uint32_t h, bool usealpha, uint32_t col);
		pyxieTexture(uint8_t* img, const char* name);
		pyxieTexture(pyxieTexture* org);
		~pyxieTexture();

		void Build();
		void Initialize();
		void Clone(bool afterFinishBuild);
		RESOURCETYPE ResourceType() { return TEXTURETYPE; }

		inline TextureHandle GetTextureHandle(){
			WaitInitialize();
			return texHandle;
		}
		inline uint32_t GetTextureType(){
			WaitInitialize();
			return textureType;
		}
		inline uint32_t GetTextureWidth() {
			WaitBuild();
			return width;
		}
		inline uint32_t GetTextureHeight() { 
			WaitBuild();
			return height;
		}
		inline uint32_t GetNumMips() {
			WaitBuild();
			return numMips;
		}
		void SetSamplerState(SamplerState state);


#if defined __ENABLE_SUSPEND_RECOVER__
		virtual bool Restore();
		virtual bool Release();
#endif
	private:
		void ClearMember();
	};
}
