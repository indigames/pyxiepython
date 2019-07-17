#include "pyxie.h"
#include "pythonResource.h"
#include "pyxieResourceCreator.h"
#include "pythonAnimator_doc_en.h"

namespace pyxie
{
	PyObject *animator_new(PyTypeObject *type, PyObject *args, PyObject *kw) {
		char* path;
		animator_obj * self = NULL;

		if (PyArg_ParseTuple(args, "s", &path)) {
			self = (animator_obj*)type->tp_alloc(type, 0);
			self->anime = pyxieResourceCreator::Instance().NewAnimator(path);
		}
		return (PyObject *)self;

	}

	void  animator_dealloc(animator_obj *self)
	{
		self->anime->DecReference();
		Py_TYPE(self)->tp_free(self);

	}

	PyObject *animator_str(animator_obj *self)
	{
		char buf[64];
		pyxie_snprintf(buf, 64, "animator object");
		return _PyUnicode_FromASCII(buf, strlen(buf));
	}

	PyObject *animator_getLooping(animator_obj *self)
	{
		return PyBool_FromLong((long)self->anime->IsLoop());
	}

	int animator_setLooping(animator_obj *self, PyObject *value)
	{
		if (value == NULL || !PyBool_Check(value)) {
			PyErr_SetString(PyExc_TypeError, "loop must be set to a bool object");
			return -1;
		}
		int result = PyObject_IsTrue(value);
		self->anime->SetLoop((bool)result);
		return 0;
	}

	PyObject *animator_getEvalTime(animator_obj *self)
	{
		return PyFloat_FromDouble((double)self->anime->GetEvalTime());
	}
	int animator_setEvalTime(animator_obj *self, PyObject *value)
	{
		if (value == NULL || !PyFloat_Check(value)) {
			PyErr_SetString(PyExc_TypeError, "evalTime must be set to a float object");
			return -1;
		}
		self->anime->SetEvalTime((float)PyFloat_AsDouble(value));
		return 0;
	}

	PyObject *animator_getTotalEvalTime(animator_obj *self)
	{
		return PyFloat_FromDouble((double)self->anime->GetTotalEvalTime());
	}
	int animator_setTotalEvalTime(animator_obj *self, PyObject *value)
	{
		if (value == NULL || !PyFloat_Check(value)) {
			PyErr_SetString(PyExc_TypeError, "totalEvalTime must be set to a float object");
			return -1;
		}
		self->anime->SetTotalEvalTime((float)PyFloat_AsDouble(value));
		return 0;
	}

	PyObject *animator_getStartTime(animator_obj *self)
	{
		return PyFloat_FromDouble((double)self->anime->GetStartTime());
	}
	int animator_setStartTime(animator_obj *self, PyObject *value)
	{
		if (value == NULL || !PyFloat_Check(value)) {
			PyErr_SetString(PyExc_TypeError, "startTime must be set to a float object");
			return -1;
		}
		self->anime->SetStartTime((float)PyFloat_AsDouble(value));
		return 0;
	}

	PyObject *animator_getEndTime(animator_obj *self)
	{
		return PyFloat_FromDouble((double)self->anime->GetEndTime());
	}
	int animator_setEndTime(animator_obj *self, PyObject *value)
	{
		if (value == NULL || !PyFloat_Check(value)) {
			PyErr_SetString(PyExc_TypeError, "endTime must be set to a float object");
			return -1;
		}
		self->anime->SetEndTime((float)PyFloat_AsDouble(value));
		return 0;
	}

	PyObject *animator_getSpeed(animator_obj *self)
	{
		return PyFloat_FromDouble((double)self->anime->GetSpeed());
	}
	int animator_setSpeed(animator_obj *self, PyObject *value)
	{
		if (value == NULL || !PyFloat_Check(value)) {
			PyErr_SetString(PyExc_TypeError, "speed must be set to a float object");
			return -1;
		}
		self->anime->SetSpeed((float)PyFloat_AsDouble(value));
		return 0;
	}

	PyObject *animator_getDefaultEndtime(animator_obj *self)
	{
		return PyFloat_FromDouble((double)self->anime->GetDefaultEndtime());
	}

	PyObject *animator_getElapsedTime(animator_obj *self)
	{
		return PyFloat_FromDouble((double)self->anime->GetElapsedTime());
	}

	PyObject *animator_getGetCorrectedEvalTime(animator_obj *self)
	{
		return PyFloat_FromDouble((double)self->anime->GetEvalTime());
	}


	PyObject *animator_rewind(animator_obj *self)
	{
		self->anime->Rewind();
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyMethodDef animator_methods[] = {
		{ "rewind", (PyCFunction)animator_rewind, METH_NOARGS, rewind_doc },
		{ NULL,	NULL }
	};
	PyGetSetDef animator_getsets[] = {
		{ const_cast<char*>("loop"),             (getter)animator_getLooping, (setter)animator_setLooping,loop_doc, NULL },
		{ const_cast<char*>("evalTime"),         (getter)animator_getEvalTime, (setter)animator_setEvalTime,evalTime_doc, NULL },
		{ const_cast<char*>("totalEvalTime"),    (getter)animator_getTotalEvalTime, (setter)animator_setTotalEvalTime,totalEvalTime_doc, NULL },
		{ const_cast<char*>("startTime"),        (getter)animator_getStartTime, (setter)animator_setStartTime,startTime_doc, NULL },
		{ const_cast<char*>("endTime"),          (getter)animator_getEndTime, (setter)animator_setEndTime,endTime_doc, NULL },
		{ const_cast<char*>("speed"),            (getter)animator_getSpeed, (setter)animator_setSpeed,speed_doc, NULL },
		{ const_cast<char*>("defaultEndTime"),   (getter)animator_getDefaultEndtime, NULL,defaultEndTime_doc, NULL },
		{ const_cast<char*>("elapsedTime"),      (getter)animator_getElapsedTime, NULL,elapsedTime_doc, NULL },
		{ const_cast<char*>("correctedEvalTime"),(getter)animator_getGetCorrectedEvalTime, NULL, correctedEvalTime_doc, NULL },
		{ NULL, NULL }
	};

	PyTypeObject AnimatorType = {
		PyVarObject_HEAD_INIT(NULL, 0)
		"pyxie.animator",					/* tp_name */
		sizeof(animator_obj),               /* tp_basicsize */
		0,                                  /* tp_itemsize */
		(destructor)animator_dealloc,		/* tp_dealloc */
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
		(reprfunc)animator_str,             /* tp_str */
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
		animator_methods,					/* tp_methods */
		0,                                  /* tp_members */
		animator_getsets,                   /* tp_getset */
		0,                                  /* tp_base */
		0,                                  /* tp_dict */
		0,                                  /* tp_descr_get */
		0,                                  /* tp_descr_set */
		0,                                  /* tp_dictoffset */
		0,                                  /* tp_init */
		0,                                  /* tp_alloc */
		animator_new,						/* tp_new */
		0,									/* tp_free */
	};
}