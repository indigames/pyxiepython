///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once
#include "pyxieObject.h"

namespace pyxie {

	class PYXIE_EXPORT pyxieTime : public pyxieObject
	{
		pyxieTime();
	public:
		static void New();
		static void Delete();
		static pyxieTime& Instance();

		double GetCurrentTime();
		double GetElapsedTime();
		double GetCPUTime();
		void Update();
	};
}
