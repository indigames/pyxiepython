#pragma once
#include "pyxieObject.h"
namespace pyxie {
	class pyxieCamera;
	class pyxieShowcase;

	class Backyard : public pyxieObject {
		static Backyard* instance;
	public:
		static void New() { if(!instance) instance = new Backyard; }
		static void Delete();
		static Backyard& Instance();

		~Backyard();
		void WakeBoth();
		void SyncMain();
		void SyncPython();
		void RenderRequest(pyxieCamera* camera, pyxieShowcase* showcase);
		void Render();
	};
}
