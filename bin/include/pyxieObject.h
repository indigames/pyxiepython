///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once

#include "pyxieTypes.h"
#include "pyxieUtilities.h"

namespace pyxie {

	class PYXIE_EXPORT pyxieObject {
	public:
		pyxieObject();
		virtual ~pyxieObject();
	protected:
		void* operator new(size_t size) {
			return PYXIE_MALLOC(size);
		}
		void operator delete(void* p) {
			PYXIE_FREE(p);
		}
		void* operator new[](size_t size) {
			return PYXIE_MALLOC(size);
		}
			void operator delete[](void* p) {
			PYXIE_FREE(p);
		}
	};
#define	PYXIE_SAFE_DELETE(p)		{ if(p){delete (p); (p) = 0;} }
#define	PYXIE_SAFE_DELETE_ARRAY(p)	{ if(p){delete [] (p); (p) = 0;} }
}

