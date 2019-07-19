#include "pyxie.h"
#include "pythonResource.h"
#include "pyxieResourceCreator.h"
#include "pythonShowcase_doc_en.h"

namespace pyxie
{
	PyObject *showcase_new(PyTypeObject *type, PyObject *args, PyObject *kw) {
		showcase_obj * self = NULL;
		self = (showcase_obj*)type->tp_alloc(type, 0);
		self->showcase = pyxieResourceCreator::Instance().NewShowcase();
		return (PyObject *)self;
	}

	void  showcase_dealloc(showcase_obj *self)
	{
		Py_TYPE(self)->tp_free(self);
	}

	PyObject *showcase_str(showcase_obj *self)
	{
		char buf[64];
		pyxie_snprintf(buf, 64, "showcase object");
		return _PyUnicode_FromASCII(buf, strlen(buf));
	}


	static PyObject *shocase_Add(showcase_obj *self, PyObject *args)
	{
		PyObject* pyobj = NULL;
		if (PyArg_ParseTuple(args, "O", &pyobj))
		{
			if (pyobj->ob_type == &FigureType || pyobj->ob_type == &CameraType || pyobj->ob_type == &EnvironmentType || pyobj->ob_type == &EditableFigureType) {
				self->showcase->Add(((resource_obj*)pyobj)->res);
			}
			else return NULL;
		}
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject *shocase_Remove(showcase_obj *self, PyObject *args)
	{
		PyObject* pyobj = NULL;
		if (PyArg_ParseTuple(args, "O", &pyobj))
		{
			if (pyobj->ob_type == &FigureType || pyobj->ob_type == &CameraType || pyobj->ob_type == &EnvironmentType) {
				self->showcase->Remove(((resource_obj*)pyobj)->res);
			}
			else return NULL;
		}
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* shocase_Clear(showcase_obj* self)
	{
		self->showcase->Clear();
		Py_INCREF(Py_None);
		return Py_None;
	}



	PyMethodDef showcase_methods[] = {
		{ "add", (PyCFunction)shocase_Add, METH_VARARGS,add_doc },
		{ "remove", (PyCFunction)shocase_Remove, METH_VARARGS,remove_doc },
		{ "clear", (PyCFunction)shocase_Clear, METH_NOARGS,clear_doc },
		{ NULL,	NULL }
	};
	PyGetSetDef showcase_getsets[] = {
		{ NULL, NULL }
	};

	PyTypeObject ShowcaseType = {
		PyVarObject_HEAD_INIT(NULL, 0)
		"pyxie.showcase",					/* tp_name */
		sizeof(showcase_obj),               /* tp_basicsize */
		0,                                  /* tp_itemsize */
		(destructor)showcase_dealloc,		/* tp_dealloc */
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
		(reprfunc)showcase_str,             /* tp_str */
		0,                                  /* tp_getattro */
		0,                                  /* tp_setattro */
		0,                                  /* tp_as_buffer */
		Py_TPFLAGS_DEFAULT,					/* tp_flags */
		showcase_doc,						/* tp_doc */
		0,									/* tp_traverse */
		0,                                  /* tp_clear */
		0,                                  /* tp_richcompare */
		0,                                  /* tp_weaklistoffset */
		0,									/* tp_iter */
		0,									/* tp_iternext */
		showcase_methods,					/* tp_methods */
		0,                                  /* tp_members */
		showcase_getsets,                   /* tp_getset */
		0,                                  /* tp_base */
		0,                                  /* tp_dict */
		0,                                  /* tp_descr_get */
		0,                                  /* tp_descr_set */
		0,                                  /* tp_dictoffset */
		0,                                  /* tp_init */
		0,                                  /* tp_alloc */
		showcase_new,						/* tp_new */
		0,									/* tp_free */
	};
}