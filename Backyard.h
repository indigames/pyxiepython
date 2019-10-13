#pragma once
#include "pyxieObject.h"
namespace pyxie {
	class pyxieCamera;
	class pyxieShowcase;
	class pyxieRenderTarget;

	class Backyard : public pyxieObject {
		static Backyard* instance;
	public:
		static void New() { if(!instance) instance = new Backyard; }
		static void Delete();
		static Backyard& Instance();

		~Backyard();
		void WakeBoth();
		void SyncMain();
		void SyncPython(bool swapframe=true);
		void RenderRequest(pyxieCamera* camera, pyxieShowcase* showcase, pyxieRenderTarget* offscreen, bool clearColor, bool clearDepth, const float* color);
		void Render();
		void QuitPython();

		void SetFinishProgram(bool finish);
		bool IsProgramRunning();

		void UpdateImageRequest(void* tex);
	};
}
