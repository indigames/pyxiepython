#include "pyxie.h"
#include "pythonResource.h"
#include "pyxieResourceCreator.h"
#include "structmember.h"

namespace pyxie
{
	const int numStatics = 128;
	static const uint16_t static_statics[numStatics] = {
		0,
		1,
		2,
		ATTRIBUTE_ID_POSITION,
		ATTRIBUTE_ID_NORMAL,
		ATTRIBUTE_ID_TANGENT,
		ATTRIBUTE_ID_BINORMAL,
		ATTRIBUTE_ID_UV0,
		ATTRIBUTE_ID_UV1,
		ATTRIBUTE_ID_UV2,
		ATTRIBUTE_ID_UV3,
		ATTRIBUTE_ID_COLOR,
		ATTRIBUTE_ID_BLENDINDICES,
		ATTRIBUTE_ID_BLENDWEIGHT,
		ATTRIBUTE_ID_PSIZE,
		GL_BYTE,
		GL_UNSIGNED_BYTE,
		GL_SHORT,
		GL_UNSIGNED_SHORT,
		GL_INT,
		GL_UNSIGNED_INT,
		GL_FLOAT,
		GL_2_BYTES,
		GL_3_BYTES,
		GL_4_BYTES,
		GL_DOUBLE,
		GL_HALF_FLOAT,
		SamplerState::WRAP,
		SamplerState::MIRROR,
		SamplerState::CLAMP,
		SamplerState::BORDER,
		SamplerState::LINEAR,
		SamplerState::NEAREST,
		SamplerState::NEAREST_MIPMAP_NEAREST,
		SamplerState::LINEAR_MIPMAP_NEAREST,
		SamplerState::NEAREST_MIPMAP_LINEAR,
		SamplerState::LINEAR_MIPMAP_LINEAR
	};

	typedef struct {
		PyObject_HEAD
			uint16_t statics[numStatics];
	} statics_obj;


	PyObject* statics_new(PyTypeObject* type, PyObject* args, PyObject* kw) {
		statics_obj* self = (statics_obj*)type->tp_alloc(type, 0);
		memcpy(self->statics, static_statics, numStatics * sizeof(uint16_t));
		return (PyObject*)self;
	}

	void  statics_dealloc(statics_obj* self) {
		Py_TYPE(self)->tp_free(self);
	}

	PyObject* statics_str(statics_obj* self) {
		char buf[64];
		pyxie_snprintf(buf, 64, "static values used in pyxie.");
		return _PyUnicode_FromASCII(buf, strlen(buf));
	}

	static PyMemberDef statics_members[] = {
		{"PLATFORM_PC",		T_SHORT, offsetof(statics_obj, statics) + 0, READONLY, NULL},
		{"PLATFORM_IOS",	T_SHORT, offsetof(statics_obj, statics) + 2, READONLY, NULL},
		{"PLATFORM_ANDROID",T_SHORT, offsetof(statics_obj, statics) + 4, READONLY, NULL},
		{"ATTRIBUTE_ID_POSITION",T_SHORT, offsetof(statics_obj, statics) + 6, READONLY, NULL},
		{"ATTRIBUTE_ID_NORMAL",T_SHORT, offsetof(statics_obj, statics) + 8, READONLY, NULL},
		{"ATTRIBUTE_ID_TANGENT",T_SHORT, offsetof(statics_obj, statics) + 10, READONLY, NULL},
		{"ATTRIBUTE_ID_BINORMAL",T_SHORT, offsetof(statics_obj, statics) + 12, READONLY, NULL},
		{"ATTRIBUTE_ID_UV0",T_SHORT, offsetof(statics_obj, statics) + 14, READONLY, NULL},
		{"ATTRIBUTE_ID_UV1",T_SHORT, offsetof(statics_obj, statics) + 16, READONLY, NULL},
		{"ATTRIBUTE_ID_UV2",T_SHORT, offsetof(statics_obj, statics) + 18, READONLY, NULL},
		{"ATTRIBUTE_ID_UV3",T_SHORT, offsetof(statics_obj, statics) + 20, READONLY, NULL},
		{"ATTRIBUTE_ID_COLOR",T_SHORT, offsetof(statics_obj, statics) + 22, READONLY, NULL},
		{"ATTRIBUTE_ID_BLENDINDICES",T_SHORT, offsetof(statics_obj, statics) + 24, READONLY, NULL},
		{"ATTRIBUTE_ID_BLENDWEIGHT",T_SHORT, offsetof(statics_obj, statics) + 26, READONLY, NULL},
		{"ATTRIBUTE_ID_PSIZE",T_SHORT, offsetof(statics_obj, statics) + 28, READONLY, NULL},
		{"GL_BYTE",T_SHORT, offsetof(statics_obj, statics) + 30, READONLY, NULL},
		{"GL_UNSIGNED_BYTE",T_SHORT, offsetof(statics_obj, statics) + 32, READONLY, NULL},
		{"GL_SHORT",T_SHORT, offsetof(statics_obj, statics) + 34, READONLY, NULL},
		{"GL_UNSIGNED_SHORT",T_SHORT, offsetof(statics_obj, statics) + 36, READONLY, NULL},
		{"GL_INT",T_SHORT, offsetof(statics_obj, statics) + 38, READONLY, NULL},
		{"GL_UNSIGNED_INT",T_SHORT, offsetof(statics_obj, statics) + 40, READONLY, NULL},
		{"GL_FLOAT",T_SHORT, offsetof(statics_obj, statics) + 42, READONLY, NULL},
		{"GL_2_BYTES",T_SHORT, offsetof(statics_obj, statics) + 44, READONLY, NULL},
		{"GL_3_BYTES",T_SHORT, offsetof(statics_obj, statics) + 46, READONLY, NULL},
		{"GL_4_BYTES",T_SHORT, offsetof(statics_obj, statics) + 48, READONLY, NULL},
		{"GL_DOUBLE",T_SHORT, offsetof(statics_obj, statics) + 50, READONLY, NULL},
		{"GL_HALF_FLOAT",T_SHORT, offsetof(statics_obj, statics) + 52, READONLY, NULL},
		{"SAMPLERSTATE_WRAP",T_SHORT, offsetof(statics_obj, statics) + 54, READONLY, NULL},
		{"SAMPLERSTATE_MIRROR",T_SHORT, offsetof(statics_obj, statics) + 56, READONLY, NULL},
		{"SAMPLERSTATE_CLAMP",T_SHORT, offsetof(statics_obj, statics) + 58, READONLY, NULL},
		{"SAMPLERSTATE_BORDER",T_SHORT, offsetof(statics_obj, statics) + 60, READONLY, NULL},
		{"SAMPLERSTATE_LINEAR",T_SHORT, offsetof(statics_obj, statics) + 62, READONLY, NULL},
		{"SAMPLERSTATE_NEAREST",T_SHORT, offsetof(statics_obj, statics) + 64, READONLY, NULL},
		{"SAMPLERSTATE_NEAREST_MIPMAP_NEAREST",T_SHORT, offsetof(statics_obj, statics) + 68, READONLY, NULL},
		{"SAMPLERSTATE_LINEAR_MIPMAP_NEAREST",T_SHORT, offsetof(statics_obj, statics) + 70, READONLY, NULL},
		{"SAMPLERSTATE_NEAREST_MIPMAP_LINEAR",T_SHORT, offsetof(statics_obj, statics) + 72, READONLY, NULL},
		{"SAMPLERSTATE_LINEAR_MIPMAP_LINEAR",T_SHORT, offsetof(statics_obj, statics) + 74, READONLY, NULL},
	{NULL}  /* Sentinel */
	};

	PyTypeObject StaticsType = {
		PyVarObject_HEAD_INIT(NULL, 0)
		"pyxie.statics",					/* tp_name */
		sizeof(statics_obj),			    /* tp_basicsize */
		0,                                  /* tp_itemsize */
		(destructor)statics_dealloc,		/* tp_dealloc */
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
		(reprfunc)statics_str,				/* tp_str */
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
		0,									/* tp_methods */
		statics_members,                   /* tp_members */
		0,				                    /* tp_getset */
		0,                                  /* tp_base */
		0,                                  /* tp_dict */
		0,                                  /* tp_descr_get */
		0,                                  /* tp_descr_set */
		0,                                  /* tp_dictoffset */
		0,                                  /* tp_init */
		0,                                  /* tp_alloc */
		statics_new,						/* tp_new */
		0,									/* tp_free */
	};

}




