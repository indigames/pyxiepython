#include "pyxie.h"
#include "pythonResource.h"
#include "pyxieResourceCreator.h"
#include "pyxieShaderGen.h"
#include "structmember.h"

namespace pyxie
{

	PyObject * shaderGen_new(PyTypeObject *type, PyObject *args, PyObject *kw) {

		shaderGen_obj * self = NULL;
		self = (shaderGen_obj*)type->tp_alloc(type, 0);
		self->shaderDesc = pyxieResourceCreator::Instance().NewShaderDescriptor();

		self->MapChannel_None = pyxieShaderDescriptor::NoMapChannel;
		self->MapChannel_DiffuseAlpha = pyxieShaderDescriptor::DiffuseMapAlphaChannel;
		self->MapChannel_DiffuseRed = pyxieShaderDescriptor::DiffuseMapRedChannel;
		self->MapChannel_NormalAlpha = pyxieShaderDescriptor::NormalMapAlphaChannel;
		self->MapChannel_NormalRed = pyxieShaderDescriptor::NormalMapRedChannel;
		self->MapChannel_LightAlpha = pyxieShaderDescriptor::LightMapAlphaChannel;
		self->MapChannel_LightRed = pyxieShaderDescriptor::LightMapRedChannel;
		self->MapChannel_VertexColorRed = pyxieShaderDescriptor::VertexColorRedChanel;
		self->MapChannel_VertexColorAlpha = pyxieShaderDescriptor::VertexColorAlphaChanel;

		return (PyObject *)self;
	}

	void  shaderGen_dealloc(shaderGen_obj *self)
	{
		pyxieResourceCreator::Instance().DeleteShaderDescriptor(self->shaderDesc);
		Py_TYPE(self)->tp_free(self);
	}

	PyObject * shaderGen_str(shaderGen_obj *self)
	{
		self->shaderDesc->InterpolationDesc();
		std::stringstream outStream;
		std::stringstream tmpStream;
		self->shaderDesc->WriteShaderDefinition("GL3VP", tmpStream);
		pyxie::pyxieShaderGen gen;
		gen.Setup("GL3VP", tmpStream);
		gen.Precompile(tmpStream.str(), outStream);
		outStream << "\n?-?\n";
		tmpStream.str("");
		self->shaderDesc->WriteShaderDefinition("GL3FP", tmpStream);
		gen.Setup("GL3FP", tmpStream);
		gen.Precompile(tmpStream.str(), outStream);
		char buf[2018];
		pyxie_snprintf(buf, 2018, outStream.str().c_str());
		return _PyUnicode_FromASCII(buf, strlen(buf));
	}

	static PyObject* shaderGen_SetBoneCondition(shaderGen_obj* self, PyObject* args) {
		int numBoneINfluence = 0;
		int totalBone = 0;
		if (!PyArg_ParseTuple(args, "ii", &numBoneINfluence, &totalBone))return NULL;
		self->shaderDesc->SetBoneCondition(numBoneINfluence, totalBone);
		Py_INCREF(Py_None);
		return Py_None;
	}
	static PyObject* shaderGen_SetSpecular(shaderGen_obj* self, PyObject* args) {
		int enable = 0;
		int textureChannel = 0;
		if (!PyArg_ParseTuple(args, "ii", &enable, &textureChannel))return NULL;
		self->shaderDesc->SetSpecular((bool)(enable), (pyxieShaderDescriptor::ReferenceMapChannel)textureChannel);
		Py_INCREF(Py_None);
		return Py_None;
	}
	static PyObject* shaderGen_SetAmbientOcclusion(shaderGen_obj* self, PyObject* args) {
		int textureChannel = 0;
		if (!PyArg_ParseTuple(args, "i", &textureChannel))return NULL;
		self->shaderDesc->SetAmbientOcclusion((pyxieShaderDescriptor::ReferenceMapChannel)textureChannel);
		Py_INCREF(Py_None);
		return Py_None;
	}
	static PyObject* shaderGen_SetColorTextureUVChannel(shaderGen_obj* self, PyObject* args) {
		int rgb;
		int a;
		if (!PyArg_ParseTuple(args, "ii", &rgb, &a))return NULL;
		self->shaderDesc->SetColorTextureUVChannel(rgb, a);
		Py_INCREF(Py_None);
		return Py_None;
	}
	static PyObject* shaderGen_SetNormalTextureUVChannel(shaderGen_obj* self, PyObject* args) {
		int rgb;
		int a;
		if (!PyArg_ParseTuple(args, "ii", &rgb, &a))return NULL;
		self->shaderDesc->SetNormalTextureUVChannel(rgb,a);
		Py_INCREF(Py_None);
		return Py_None;
	}
	static PyObject* shaderGen_SetLightTextureUVChannel(shaderGen_obj* self, PyObject* args) {
		int rgb;
		int a;
		if (!PyArg_ParseTuple(args, "ii", &rgb, &a))return NULL;
		self->shaderDesc->SetLightTextureUVChannel(rgb,a);
		Py_INCREF(Py_None);
		return Py_None;
	}
	static PyObject* shaderGen_SetOverlayColorTextureUVChannel(shaderGen_obj* self, PyObject* args) {
		int rgb;
		int a;
		if (!PyArg_ParseTuple(args, "ii", &rgb, &a))return NULL;
		self->shaderDesc->SetOverlayColorTextureUVChannel(rgb, a);
		Py_INCREF(Py_None);
		return Py_None;
	}
	static PyObject* shaderGen_SetOverlayNormalColorTextureUVChannel(shaderGen_obj* self, PyObject* args) {
		int rgb;
		int a;
		if (!PyArg_ParseTuple(args, "ii", &rgb, &a))return NULL;
		self->shaderDesc->SetOverlayNormalColorTextureUVChannel(rgb, a);
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* shaderGen_SetCalcBinormalInShader(shaderGen_obj* self, PyObject* args) {
		int enable = 0;
		if (!PyArg_ParseTuple(args, "i", &enable))return NULL;
		self->shaderDesc->SetCalcBinormalInShader(enable);
		Py_INCREF(Py_None);
		return Py_None;
	}
	static PyObject* shaderGen_SetNumDirLamp(shaderGen_obj* self, PyObject* args) {
		int num = 0;
		if (!PyArg_ParseTuple(args, "i", &num))return NULL;
		self->shaderDesc->SetNumDirLamp(num);
		Py_INCREF(Py_None);
		return Py_None;
	}
	static PyObject* shaderGen_SetNumPointLamp(shaderGen_obj* self, PyObject* args) {
		int num = 0;
		if (!PyArg_ParseTuple(args, "i", &num))return NULL;
		self->shaderDesc->SetNumPointLamp(num);
		Py_INCREF(Py_None);
		return Py_None;
	}
	static PyObject* shaderGen_SetAmbientType(shaderGen_obj* self, PyObject* args) {
		int type;
		if (!PyArg_ParseTuple(args, "i", &type))return NULL;
		self->shaderDesc->SetAmbientType(type);
		Py_INCREF(Py_None);
		return Py_None;
	}
	static PyObject* shaderGen_SetClutLamp(shaderGen_obj* self, PyObject* args) {
		int enable = 0;
		if (!PyArg_ParseTuple(args, "i", &enable))return NULL;
		self->shaderDesc->SetClutLamp(enable);
		Py_INCREF(Py_None);
		return Py_None;
	}
	static PyObject* shaderGen_SetVertexColor(shaderGen_obj * self, PyObject* args) {
		int enable = 0;
		if (!PyArg_ParseTuple(args, "i", &enable))return NULL;
		self->shaderDesc->SetVertexColor(enable);
		Py_INCREF(Py_None);
		return Py_None;
	}
	static PyObject* shaderGen_SetVertexAlpha(shaderGen_obj* self, PyObject* args) {
		int enable = 0;
		if (!PyArg_ParseTuple(args, "i", &enable))return NULL;
		self->shaderDesc->SetVertexAlpha(enable);
		Py_INCREF(Py_None);
		return Py_None;
	}
	static PyObject* shaderGen_SetColorTexture(shaderGen_obj* self, PyObject* args) {
		int enable = 0;
		if (!PyArg_ParseTuple(args, "i", &enable))return NULL;
		self->shaderDesc->SetColorTexture(enable);
		Py_INCREF(Py_None);
		return Py_None;
	}
	static PyObject* shaderGen_SetNormalTexture(shaderGen_obj* self, PyObject* args) {
		int enable = 0;
		if (!PyArg_ParseTuple(args, "i", &enable))return NULL;
		self->shaderDesc->SetNormalTexture(enable);
		Py_INCREF(Py_None);
		return Py_None;
	}
	static PyObject* shaderGen_SetLightTexture(shaderGen_obj* self, PyObject* args) {
		int enable = 0;
		if (!PyArg_ParseTuple(args, "i", &enable))return NULL;
		self->shaderDesc->SetLightTexture(enable);
		Py_INCREF(Py_None);
		return Py_None;
	}
	static PyObject* shaderGen_SetOverlayColorTexture(shaderGen_obj* self, PyObject* args) {
		int enable = 0;
		if (!PyArg_ParseTuple(args, "i", &enable))return NULL;
		self->shaderDesc->SetOverlayColorTexture(enable);
		Py_INCREF(Py_None);
		return Py_None;
	}
	static PyObject* shaderGen_SetOverlayNormalTexture(shaderGen_obj* self, PyObject* args) {
		int enable = 0;
		if (!PyArg_ParseTuple(args, "i", &enable))return NULL;
		self->shaderDesc->SetOverlayNormalTexture(enable);
		Py_INCREF(Py_None);
		return Py_None;
	}
	static PyObject* shaderGen_SetOverlaySpecularTexture(shaderGen_obj* self, PyObject* args) {
		int enable = 0;
		if (!PyArg_ParseTuple(args, "i", &enable))return NULL;
		self->shaderDesc->SetOverlaySpecularTexture(enable);
		Py_INCREF(Py_None);
		return Py_None;
	}
	static PyObject* shaderGen_SetOverlayVertexAlpha(shaderGen_obj* self, PyObject* args) {
		int enable = 0;
		if (!PyArg_ParseTuple(args, "i", &enable))return NULL;
		self->shaderDesc->SetOverlayVertexAlpha(enable);
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyMethodDef shaderGen_methods[] = {
		{ "setBoneCondition",(PyCFunction)shaderGen_SetBoneCondition, METH_VARARGS },
		{ "setSpecular",(PyCFunction)shaderGen_SetSpecular, METH_VARARGS },
		{ "setAmbientOcclusion",(PyCFunction)shaderGen_SetAmbientOcclusion, METH_VARARGS },
		{ "setColorTextureUVChannel",(PyCFunction)shaderGen_SetColorTextureUVChannel, METH_VARARGS },
		{ "setNormalTextureUVChannel",(PyCFunction)shaderGen_SetNormalTextureUVChannel, METH_VARARGS },
		{ "setLightTextureUVChannel",(PyCFunction)shaderGen_SetLightTextureUVChannel, METH_VARARGS },
		{ "setOverlayColorTextureUVChannel",(PyCFunction)shaderGen_SetOverlayColorTextureUVChannel, METH_VARARGS },
		{ "setOverlayNormalColorTextureUVChannel",(PyCFunction)shaderGen_SetOverlayNormalColorTextureUVChannel, METH_VARARGS },
		{ "setCalcBinormalInShader",(PyCFunction)shaderGen_SetCalcBinormalInShader, METH_VARARGS },
		{ "setNumDirLamp",(PyCFunction)shaderGen_SetNumDirLamp, METH_VARARGS },
		{ "setNumPointLamp",(PyCFunction)shaderGen_SetNumPointLamp, METH_VARARGS },
		{ "setAmbientType",(PyCFunction)shaderGen_SetAmbientType, METH_VARARGS },
		{ "setClutLamp",(PyCFunction)shaderGen_SetClutLamp, METH_VARARGS },
		{ "setVertexColor",(PyCFunction)shaderGen_SetVertexColor, METH_VARARGS },
		{ "setVertexAlpha",(PyCFunction)shaderGen_SetVertexAlpha, METH_VARARGS },
		{ "setColorTexture",(PyCFunction)shaderGen_SetColorTexture, METH_VARARGS },
		{ "setNormalTexture",(PyCFunction)shaderGen_SetNormalTexture, METH_VARARGS },
		{ "setLightTexture",(PyCFunction)shaderGen_SetLightTexture, METH_VARARGS },
		{ "setOverlayColorTexture",(PyCFunction)shaderGen_SetOverlayColorTexture, METH_VARARGS },
		{ "setOverlayNormalTexture",(PyCFunction)shaderGen_SetOverlayNormalTexture, METH_VARARGS },
		{ "setOverlaySpecularTexture",(PyCFunction)shaderGen_SetOverlaySpecularTexture, METH_VARARGS },
		{ "setOverlayVertexAlpha",(PyCFunction)shaderGen_SetOverlayVertexAlpha, METH_VARARGS },
		{ NULL,	NULL }
	};


	PyMemberDef shaderGen_members[] = {
		{"MapChannel_None",T_INT, offsetof(shaderGen_obj, MapChannel_None), READONLY, NULL},
		{"MapChannel_DiffuseAlpha",T_INT, offsetof(shaderGen_obj, MapChannel_DiffuseAlpha), READONLY, NULL},
		{"MapChannel_DiffuseRed",T_INT, offsetof(shaderGen_obj, MapChannel_DiffuseRed), READONLY, NULL},
		{"MapChannel_NormalAlpha",T_INT, offsetof(shaderGen_obj, MapChannel_NormalAlpha), READONLY, NULL},
		{"MapChannel_NormalRed",T_INT, offsetof(shaderGen_obj, MapChannel_NormalRed), READONLY, NULL},
		{"MapChannel_LightAlpha",T_INT, offsetof(shaderGen_obj, MapChannel_LightAlpha), READONLY, NULL},
		{"MapChannel_LightRed",T_INT, offsetof(shaderGen_obj, MapChannel_LightRed), READONLY, NULL},
		{"MapChannel_VertexColorRed",T_INT, offsetof(shaderGen_obj, MapChannel_VertexColorRed), READONLY, NULL},
		{"MapChannel_VertexColorAlpha",T_INT, offsetof(shaderGen_obj, MapChannel_VertexColorAlpha), READONLY, NULL},
		{NULL}  /* Sentinel */
	};



	PyTypeObject ShaderGeneratorType = {
		PyVarObject_HEAD_INIT(NULL, 0)
		"pyxie.shaderGeneragtor",			/* tp_name */
		sizeof(shaderGen_obj),             /* tp_basicsize */
		0,                                  /* tp_itemsize */
		(destructor)shaderGen_dealloc,		/* tp_dealloc */
		0,                                  /* tp_print */
		0,							        /* tp_getattr */
		0,                                  /* tp_setattr */
		0,                                  /* tp_reserved */
		0,                                  /* tp_repr */
		0,					                /* tp_as_number */
		0,                                  /* tp_as_sequence */
		0,                                  /* tp_as_mapping */
		0,                                  /* tp_hash */
		0,                                  /* tp_call */
		(reprfunc)shaderGen_str,           /* tp_str */
		0,                                  /* tp_getattro */
		0,                                  /* tp_setattro */
		0,                                  /* tp_as_buffer */
		Py_TPFLAGS_DEFAULT,					/* tp_flags */
		0,									/* tp_doc */
		0,									/* tp_traverse */
		0,                                  /* tp_clear */
		0,                                  /* tp_richcompare */
		0,                                  /* tp_weaklistoffset */
		0,									/* tp_iter */
		0,									/* tp_iternext */
		shaderGen_methods,					/* tp_methods */
		shaderGen_members,                 /* tp_members */
		0,									/* tp_getset */
		0,                                  /* tp_base */
		0,                                  /* tp_dict */
		0,                                  /* tp_descr_get */
		0,                                  /* tp_descr_set */
		0,                                  /* tp_dictoffset */
		0,                                  /* tp_init */
		0,                                  /* tp_alloc */
		shaderGen_new,						/* tp_new */
		0,									/* tp_free */
	};

}