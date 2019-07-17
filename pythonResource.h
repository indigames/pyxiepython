#pragma once

#include <Python.h>
#include "pyxieAnimator.h"
#include "pyxieFigure.h"
#include "pyxieEditableFigure.h"
#include "pyxieCamera.h"
#include "pyxieEnvironmentSet.h"
#include "pyxieShowcase.h"
#include "pyxieShaderDescriptor.h"

namespace pyxie {
	typedef struct {
		PyObject_HEAD
			pyxieResource* res;
	} resource_obj;

	typedef struct {
		PyObject_HEAD
			pyxieAnimator* anime;
	} animator_obj;

	typedef struct {
		PyObject_HEAD
			pyxieFigure* figure;
	} figure_obj;

	typedef struct {
		PyObject_HEAD
		pyxieEditableFigure* editablefigure;
		int VertexAttribure_POSITION;
		int VertexAttribure_NORMAL;
		int VertexAttribure_TANGENT;
		int VertexAttribure_BINORMAL;
		int VertexAttribure_UV0;
		int VertexAttribure_UV1;
		int VertexAttribure_UV2;
		int VertexAttribure_UV3;
		int VertexAttribure_COLOR;
		int VertexAttribure_BLENDINDICES;
		int VertexAttribure_BLENDWEIGHT;
		int VertexAttribure_PSIZE;
	} editablefigure_obj;

	typedef struct {
		PyObject_HEAD
			pyxieCamera* camera;
	} camera_obj;

	typedef struct {
		PyObject_HEAD
			pyxieEnvironmentSet* envSet;
	} environment_obj;

	typedef struct {
		PyObject_HEAD
			pyxieShowcase* showcase;
	} showcase_obj;

	typedef struct {
		PyObject_HEAD
		pyxieShaderDescriptor* shaderDesc;
		int MapChannel_None;
		int MapChannel_DiffuseAlpha;
		int MapChannel_DiffuseRed;
		int MapChannel_NormalAlpha;
		int MapChannel_NormalRed;
		int MapChannel_LightAlpha;
		int MapChannel_LightRed;
		int MapChannel_VertexColorRed;
		int MapChannel_VertexColorAlpha;
	} shaderGen_obj;

	extern PyTypeObject FigureType;
	extern PyTypeObject EditableFigureType;
	extern PyTypeObject AnimatorType;
	extern PyTypeObject CameraType;
	extern PyTypeObject EnvironmentType;
	extern PyTypeObject ShowcaseType;
	extern PyTypeObject ShaderGeneratorType;

	float* pyObjToFloat(PyObject* obj, float* f, int& d);
	int pyObjToFloatArray(PyObject* obj, float* f, int numElement);

	bool ImportVMath();

	extern PyTypeObject* _Vec2Type;
	extern PyTypeObject* _Vec3Type;
	extern PyTypeObject* _Vec4Type;
	extern PyTypeObject* _QuatType;
	extern PyTypeObject* _Mat22Type;
	extern PyTypeObject* _Mat33Type;
	extern PyTypeObject* _Mat44Type;
}

