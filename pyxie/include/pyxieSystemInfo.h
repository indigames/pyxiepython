///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once

namespace pyxie
{
	class pyxieSystemInfo
	{
	public:
		static pyxieSystemInfo& Instance() { return instance; }

		enum Axis
		{
			Horizontal,
			Vertical
		};

		void SetSoftLength(float size, Axis axis);
		void SetHardSize(int w, int h);
		int GetHardWidth() { return hardWidth; }
		int GetHardHeight() { return hardHeight; }
		float GetSoftWidth() { return softWidth; }
		float GetSoftHeight() { return softHeight; }
		Axis GetBaseAxis() { return baseAxis; }
	private:
		int hardWidth;
		int hardHeight;
		float softWidth;
		float softHeight;
		Axis baseAxis;
		pyxieSystemInfo();
		static pyxieSystemInfo instance;
	};

}