///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once

#include "pyxieResource.h"
#include "pyxieTexture.h"

#include <string>

#define FNT_DEFAULT 0
#ifdef __APPLE_CC__
#define FNT_HIRAGINO 0 /*HiraKakuProN-W3*/
#define FNT_ARIAL 1 /*ArialMT*/
#define FNT_VERDANA 2 /*Verdana*/
#define FNT_HELVETICA 3 /*Helvetica*/
#define FNT_TIMESNEWROMAN 4 /*TimesNewRomanPSMT*/
#define FNT_KAILASA 5 /*Kailasa*/
#define FNT_GEORGIA 6 /*Georgia*/
#elif defined WIN32
#define FNT_HIRAGINO 0 /*ƒƒCƒŠƒI*/
#define FNT_ARIAL 1 /*Arial*/
#define FNT_VERDANA 2 /*Verdana*/
#define FNT_HELVETICA 3 /*Helvetica*/
#define FNT_TIMESNEWROMAN 4 /*Times new Roman*/
#define FNT_KAILASA 5 /*Kailasa*/
#define FNT_GEORGIA 6 /*Georgia*/
#elif defined __ANDROID__
#define FNT_HIRAGINO 0
#define FNT_ARIAL 1
#define FNT_VERDANA 2
#define FNT_HELVETICA 3
#define FNT_TIMESNEWROMAN 4
#define FNT_KAILASA 5
#define FNT_GEORGIA 6
#endif

namespace pyxie
{
	class pyxieTextRenderer : public pyxieResource
	{
		TextureHandle texHandle;
		pyxieTexture* tex;
		uint32_t fontSize;
		int32_t width;
		int32_t height;
		uint32_t linePitch;
		uint32_t scrOffsX;
		uint32_t scrOffsY;
		uint32_t fontCode;
		int32_t bordering;
		bool allmonospace;
		uint8_t* texbuf;
		uint32_t cursorx;
		uint32_t cursory;
		uint8_t* fgdbuf;
		uint8_t* bgdbuf;
		int32_t chPitch;
		bool rgbamode;
		uint32_t rgbacolor;
		bool forcebrdcolor;
		uint32_t brdcolor;
	public:
		pyxieTextRenderer(uint32_t tw, uint32_t th, uint32_t fsize, uint32_t linepitch = 0,
			uint32_t offsetx = 0, uint32_t offsety = 0, uint32_t font = FNT_DEFAULT,
			bool forcemonospace = false, int32_t pitch = 0, bool usergb = false);
		~pyxieTextRenderer();

		pyxieTexture* GetTexture() { return tex; }
		bool SetTextureSize(uint32_t tw, uint32_t th)
		{
			//if (IsInitialized(true)) return false;
#if defined(__ANDROID__)
			if (IsAndroidFixTexAlign())
			{
				uint32_t wrem = tw % 8;
				uint32_t hrem = th % 8;
				if (wrem > 0) tw = ((tw >> 3) + 1) << 3;
				if (hrem > 0) th = ((th >> 3) + 1) << 3;
			}
#endif
			width = tw;
			height = th;
			return true;
		}
		uint32_t GetTextureWidth() { return width; }
		uint32_t GetTextureHeight() { return height; }

		void GetTextSize(const char* string, int32_t* textw, int32_t* texth);
		void InsertLFByWidth(std::string& instr, uint32_t maxwidth, std::string& outstr);

		void SetBordering(int32_t dotnum);

		enum TextAlign
		{
			TXT_ALIGN_LEFT,
			TXT_ALIGN_CENTER,
			TXT_ALIGN_RIGHT
		};

		void SetTextColor(uint8_t r, uint8_t g, uint8_t b);
		void SetBorderColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		void SetTextFont(uint32_t fontcode);

		bool SetText(const char* string, TextAlign align = TXT_ALIGN_LEFT);
		bool AddText(const char* string);

		inline bool IsRGBAMode() { return rgbamode; }

#if defined __ANDROID__
		virtual bool Restore();
		virtual bool Release();
		bool IsRestore() { return restore; }
		bool restore;
		bool IsAndroidFixTexAlign();
#endif

	private:
		void Char2Bitmap(const char* ch, uint8_t** outbitmap, int32_t* outw, int32_t* outh);
		void Clear();
		void FlushText();

		struct str_bitmap_t {
			uint8_t* bitmap;
			int32_t w;
			int32_t h;
		};
		uint32_t Strs2Bitmaps(const char* utf8str, str_bitmap_t** strbitmaps);
		void PlaceBitmap(str_bitmap_t* bitmap);
		void ClipBitmap(str_bitmap_t* bitmap, TextAlign align);
		void Bitmap2Fontface(str_bitmap_t* bitmap);
		void Bitmap2Bordering(str_bitmap_t* bitmap);
		void MergeLayers();
		void AdjustCursorPre(str_bitmap_t* bitmap, TextAlign align);
		void AdjustCursorPost(str_bitmap_t* bitmap);
		uint32_t FirstCharFromUTF8Z(const char* utf8str, char** oututf8zch);
	protected:
		virtual bool FinishInitialize();
	};
}
