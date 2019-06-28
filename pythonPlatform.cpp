#include "pyxie.h"
#include "pythonResource.h"
#include "pyxieResourceCreator.h"
#include "structmember.h"

namespace pyxie
{
	PyObject* platform_new(PyTypeObject* type, PyObject* args, PyObject* kw) {
		platform_obj* self = (platform_obj*)type->tp_alloc(type, 0);
		self->PC = 0;
		self->iOS = 1;
		self->Android = 2;
		return (PyObject*)self;
	}

	void  platform_dealloc(platform_obj* self){
		Py_TYPE(self)->tp_free(self);
	}

	PyObject * platform_str(platform_obj* self){
		char buf[64];
		pyxie_snprintf(buf, 64, "Target platform of pyxie.");
		return _PyUnicode_FromASCII(buf, strlen(buf));
	}

	static PyMemberDef platform_members[] = {
		{"PC",		T_INT, offsetof(platform_obj, PC), READONLY, NULL},
		{"iOS",		T_INT, offsetof(platform_obj, iOS), READONLY, NULL},
		{"Android",	T_INT, offsetof(platform_obj, Android), READONLY, NULL},
		{NULL}  /* Sentinel */
	};

	PyTypeObject PlatformType = {
		PyVarObject_HEAD_INIT(NULL, 0)
		"pyxie.platform",					/* tp_name */
		sizeof(platform_obj),			    /* tp_basicsize */
		0,                                  /* tp_itemsize */
		(destructor)platform_dealloc,		/* tp_dealloc */
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
		(reprfunc)platform_str,				/* tp_str */
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
		platform_members,                   /* tp_members */
		0,				                    /* tp_getset */
		0,                                  /* tp_base */
		0,                                  /* tp_dict */
		0,                                  /* tp_descr_get */
		0,                                  /* tp_descr_set */
		0,                                  /* tp_dictoffset */
		0,                                  /* tp_init */
		0,                                  /* tp_alloc */
		platform_new,						/* tp_new */
		0,									/* tp_free */
	};

}




