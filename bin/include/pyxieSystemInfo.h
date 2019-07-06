///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once
#include "pyxieTypes.h"

namespace pyxie
{
	class PYXIE_EXPORT pyxieSystemInfo
	{
	public:
		static pyxieSystemInfo& Instance();

		void SetGemeScreenSize(float size);
		void SetDeviceScreen(int w, int h);
		int GetDeviceW() { return devW; }
		int GetDeviceH() { return devH; }
		float GetGameW() { return gameW; }
		float GetGameH() { return gameH; }
		int GameToDevW(float w);
		int GameToDevH(float h);
	private:
		pyxieSystemInfo();
		int devW;
		int devH;
		float gameW;
		float gameH;
		float gameLength;
	};

}