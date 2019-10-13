#pragma once
#include "pyxieObject.h"
#include "pyxieShaderDescriptor.h"


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
	class pyxieRenderTarget;
	class pyxieShader;

	class PYXIE_EXPORT pyxieResourceCreator : public pyxieObject {
	public:
		static pyxieResourceCreator& Instance();
		pyxieFigure* NewFigure(const char* path);
		pyxieEditableFigure* NewEditableFigure(const char* path);
		pyxieAnimator* NewAnimator(const char* path);
		pyxieCamera* NewCamera(const char* name, pyxieFigure* parent);
		pyxieEnvironmentSet* NewEnvironmentSet(pyxieFigure* parent);
		pyxieTexture* NewTexture(const char* path);
		pyxieTexture* NewTexture(const char* name, const char* pixels, int w, int h, bool alpha);
		pyxieShowcase* NewShowcase();
		pyxieRenderTarget* NewRenderTarget(pyxieTexture* colorTexture, bool useDepth, bool useStencil);
		pyxieShader* NewShader(pyxieShaderDescriptor& desc);

		pyxieShaderDescriptor* NewShaderDescriptor();
		void DeleteShaderDescriptor(pyxieShaderDescriptor* v);
	private:
		pyxieResourceCreator(){}
	};
}
