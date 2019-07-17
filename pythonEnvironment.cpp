#include "pyxie.h"
#include "pythonResource.h"
#include "pythonEnvironment_doc_en.h"

namespace pyxie
{

	PyObject *environment_new(PyTypeObject *type, PyObject *args, PyObject *kw) {
		environment_obj * self = NULL;
		return (PyObject *)self;
	}

	void  environment_dealloc(environment_obj *self)
	{
		Py_TYPE(self)->tp_free(self);
	}

	PyObject *environment_str(environment_obj *self)
	{
		char buf[64];
		pyxie_snprintf(buf, 64, "environment object");
		return _PyUnicode_FromASCII(buf, strlen(buf));
	}

	PyMethodDef environment_methods[] = {
		{ NULL,	NULL }
	};
	PyGetSetDef environment_getsets[] = {
		{ NULL, NULL }
	};

	PyTypeObject EnvironmentType = {
		PyVarObject_HEAD_INIT(NULL, 0)
		"environment",				/* tp_name */
		sizeof(environment_obj),            /* tp_basicsize */
		0,                                  /* tp_itemsize */
		(destructor)environment_dealloc,	/* tp_dealloc */
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
		(reprfunc)environment_str,          /* tp_str */
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
		environment_methods,				/* tp_methods */
		0,                                  /* tp_members */
		environment_getsets,                /* tp_getset */
		0,                                  /* tp_base */
		0,                                  /* tp_dict */
		0,                                  /* tp_descr_get */
		0,                                  /* tp_descr_set */
		0,                                  /* tp_dictoffset */
		0,                                  /* tp_init */
		0,                                  /* tp_alloc */
		environment_new,					/* tp_new */
		0,									/* tp_free */
	};


}