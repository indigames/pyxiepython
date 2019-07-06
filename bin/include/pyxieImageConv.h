#pragma once
#include <string>
#include "pyxieTypes.h"

namespace pyxie
{
	class PYXIE_EXPORT ImageConv
	{
		std::string inputpath;
		std::string outputpath;
		int targetPlatform;
		bool isNormalmap;
		bool isHighQuality;
		bool isWrapRepeat;
		bool isNoMipmaps;
		bool isRangeScale;
		bool isRGBMformat;
		bool isAutoDetectAlpha;
		bool isOutputFormatDDS10;
		bool isConvertNormal;
		bool isOutputFormatLuminance;
		bool isOutputFormatBC1N;
		bool isOutputFormatSRGB;

	public:
		ImageConv();
		~ImageConv();
		void SetTargetPlatform(int platform) { targetPlatform = platform; }
		void SetInputFile(const char* file) { inputpath = file; }
		void SetOutputFile(const char* file) { outputpath = file; }
		void SetIsNormalmap(bool isnom = true) { isNormalmap = isnom; }
		void SetHighQuality(bool hiquality=true) { isHighQuality = hiquality; }
		void SetWrapRepeat(bool repeat = true) { isWrapRepeat = repeat; }
		void SetNoMipmaps(bool nomisp = true) { isNoMipmaps = nomisp; }
		void SetRangeScale(bool scale = true) { isRangeScale = scale; }
		void SetRGBMformat(bool rgbm = true) { isRGBMformat = rgbm; }
		void SetAutoDetectAlpha(bool autoalpha = true) { isAutoDetectAlpha = autoalpha; }
		void SetOutputFormatDDS10(bool dds10 = true) { isOutputFormatDDS10 = dds10; }
		void SetConvertNormal(bool convnom = true) { isConvertNormal = convnom; }
		void SetOutputFormatLuminance(bool luminance = true) { isOutputFormatLuminance = luminance; }
		void SetOutputFormatBC1N(bool bc1n = true) { isOutputFormatBC1N = bc1n; }
		void SetOutputFormatSRGB(bool srgb = true) { isOutputFormatSRGB = srgb; }
		bool DoConvert();
	};

}