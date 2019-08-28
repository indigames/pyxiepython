///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once

#include "pyxieObject.h"
namespace pyxie
{
	class PYXIE_EXPORT pyxieApplication : public pyxieObject
	{
	public:
		pyxieApplication();
		virtual ~pyxieApplication();

		virtual bool			onInit(DeviceHandle dh);
		virtual void			onShutdown();
		virtual bool			onUpdate();
		virtual void			onSize(int scrW, int scrH);
		virtual void			onRender();

		bool isInitialized(){return initialized;}
	protected:
		bool initialized;
	};
}