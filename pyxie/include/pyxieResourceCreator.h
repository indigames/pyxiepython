#pragma once
#include "pyxieObject.h"

namespace pyxie {

	class pyxieFigure;
	class pyxieAnimator;
	class pyxieRenderTarget;
	class pyxieEnvironmentSet;
	class pyxieCamera;
	class pyxieTexture;
	class pyxieShowcase;
	class pyxieEditableFigure;
	class pyxieShaderDescriptor;

	class PYXIE_EXPORT pyxieResourceCreator : public pyxieObject {
	public:
		static pyxieResourceCreator& Instance();
		pyxieFigure* NewFigure(const char* path);
		pyxieEditableFigure* NewEditableFigure(const char* path);
		pyxieAnimator* NewAnimator(const char* path);
		pyxieCamera* NewCamera(const char* name, pyxieFigure* parent);
		pyxieEnvironmentSet* NewEnvironmentSet(pyxieFigure* parent);
		pyxieTexture* NewTexture(const char* path);
		pyxieShowcase* NewShowcase();

		pyxieShaderDescriptor* NewShaderDescriptor();
		void DeleteShaderDescriptor(pyxieShaderDescriptor* v);
	private:
		pyxieResourceCreator(){}
	};
}
