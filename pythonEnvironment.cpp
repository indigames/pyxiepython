#include "pyxie.h"
#include "pythonResource.h"
#include "pythonEnvironment_doc_en.h"
#include "pyxieResourceCreator.h"
#include "pyVectorMath.h"

namespace pyxie
{

	PyObject *environment_new(PyTypeObject *type, PyObject *args, PyObject *kw) {
		char* name = "environment";
		environment_obj* self = NULL;
		if (PyArg_ParseTuple(args, "|s", &name)) {
			self = (environment_obj*)type->tp_alloc(type, 0);
			self->envSet = pyxieResourceCreator::Instance().NewEnvironmentSet(nullptr);
		}
		return (PyObject*)self;
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

	PyObject* environment_getambientColor(environment_obj* self)
	{
		vec_obj* v3robj = PyObject_New(vec_obj, _Vec3Type);
		if (!v3robj) return NULL;
		vmath_cpy(self->envSet->GetAmbientSkyColor().P(), 3, v3robj->v);
		v3robj->d = 3;
		return (PyObject*)v3robj;
	}

	int environment_setambientColor(environment_obj* self, PyObject* value)
	{
		int d1;
		float buff[4];
		float* v1 = pyObjToFloat((PyObject*)value, buff, d1);
		if (!v1) return NULL;
		self->envSet->SetAmbientSkyColor(*((Vec3*)v1));
		return 0;
	}

	PyObject* environment_getgroundColor(environment_obj* self)
	{
		vec_obj* v3robj = PyObject_New(vec_obj, _Vec3Type);
		if (!v3robj) return NULL;
		vmath_cpy(self->envSet->GetAmbientGroundColor().P(), 3, v3robj->v);
		v3robj->d = 3;
		return (PyObject*)v3robj;
	}

	int environment_setgroundColor(environment_obj* self, PyObject* value)
	{
		int d1;
		float buff[4];
		float* v1 = pyObjToFloat((PyObject*)value, buff, d1);
		if (!v1) return NULL;
		self->envSet->SetAmbientGroundColor(*((Vec3*)v1));
		return 0;
	}

	PyObject* environment_getambientDirection(environment_obj* self)
	{
		vec_obj* v3robj = PyObject_New(vec_obj, _Vec3Type);
		if (!v3robj) return NULL;
		vmath_cpy(self->envSet->GetAmbientDirection().P(), 3, v3robj->v);
		v3robj->d = 3;
		return (PyObject*)v3robj;
	}

	int environment_setambientDirection(environment_obj* self, PyObject* value)
	{
		int d1;
		float buff[4];
		float* v1 = pyObjToFloat((PyObject*)value, buff, d1);
		if (!v1) return NULL;
		self->envSet->SetAmbientDirection(*((Vec3*)v1));
		return 0;
	}

	static PyObject* environment_getDirectionalLampIntensity(environment_obj* self, PyObject* args) {
		int i;
		if (!PyArg_ParseTuple(args, "i", &i))return NULL;
		if (i < 0 || i>2) {
			PyErr_SetString(PyExc_TypeError, "directional lamp index is 0,1,2");
			return NULL;
		}
		float value = self->envSet->GetDirectionalLampIntensity(i);
		return PyFloat_FromDouble(value);
	}

	static PyObject* environment_setDirectionalLampIntensity(environment_obj* self, PyObject* args) {
		int i;
		float value;
		if (!PyArg_ParseTuple(args, "if", &i, &value))return NULL;
		self->envSet->SetDirectionalLampIntensity(i, value);
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* environment_getDirectionalLampColor(environment_obj* self, PyObject* args) {
		int i;
		if (!PyArg_ParseTuple(args, "i", &i))return NULL;
		if (i < 0 || i>2) {
			PyErr_SetString(PyExc_TypeError, "directional lamp index is 0,1,2");
			return NULL;
		}
		vec_obj* v3robj = PyObject_New(vec_obj, _Vec3Type);
		if (!v3robj) return NULL;
		vmath_cpy(self->envSet->GetDirectionalLampColor(i).P(), 3, v3robj->v);
		v3robj->d = 3;
		return (PyObject*)v3robj;
	}

	static PyObject* environment_setDirectionalLampColor(environment_obj* self, PyObject* args) {
		int i;
		PyObject* value;
		if (!PyArg_ParseTuple(args, "iO", &i, &value))return NULL;
		int d1;
		float buff[4];
		float* v1 = pyObjToFloat((PyObject*)value, buff, d1);
		if (!v1) return NULL;
		self->envSet->SetDirectionalLampColor(i, *((Vec3*)v1));
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* environment_getDirectionalLampDirection(environment_obj* self, PyObject* args) {
		int i;
		if (!PyArg_ParseTuple(args, "i", &i))return NULL;
		if (i < 0 || i>2) {
			PyErr_SetString(PyExc_TypeError, "directional lamp index is 0,1,2");
			return NULL;
		}
		vec_obj* v3robj = PyObject_New(vec_obj, _Vec3Type);
		if (!v3robj) return NULL;
		vmath_cpy(self->envSet->GetDirectionalLampDirection(i).P(), 3, v3robj->v);
		v3robj->d = 3;
		return (PyObject*)v3robj;
	}

	static PyObject* environment_setDirectionalLampDirection(environment_obj* self, PyObject* args) {
		int i;
		PyObject* value;
		if (!PyArg_ParseTuple(args, "iO", &i, &value))return NULL;
		int d1;
		float buff[4];
		float* v1 = pyObjToFloat((PyObject*)value, buff, d1);
		if (!v1) return NULL;
		self->envSet->SetDirectionalLampDirection(i, *((Vec3*)v1));
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* environment_getPointLampRange(environment_obj* self, PyObject* args) {
		int i;
		if (!PyArg_ParseTuple(args, "i", &i))return NULL;
		if (i < 0 || i>2) {
			PyErr_SetString(PyExc_TypeError, "directional lamp index is 0,1,2");
			return NULL;
		}
		float value = self->envSet->GetPointLampRange(i);
		return PyFloat_FromDouble(value);
	}

	static PyObject* environment_setPointLampRange(environment_obj* self, PyObject* args) {
		int i;
		float value;
		if (!PyArg_ParseTuple(args, "if", &i, &value))return NULL;
		self->envSet->SetPointLampRange(i, value);
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* environment_getPointLampIntensity(environment_obj* self, PyObject* args) {
		int i;
		if (!PyArg_ParseTuple(args, "i", &i))return NULL;
		if (i < 0 || i>2) {
			PyErr_SetString(PyExc_TypeError, "directional lamp index is 0,1,2");
			return NULL;
		}
		float value = self->envSet->GetPointLampIntensity(i);
		return PyFloat_FromDouble(value);
	}

	static PyObject* environment_setPointLampIntensity(environment_obj* self, PyObject* args) {
		int i;
		float value;
		if (!PyArg_ParseTuple(args, "if", &i, &value))return NULL;
		self->envSet->SetPointLampIntensity(i, value);
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* environment_getPointLampColor(environment_obj* self, PyObject* args) {
		int i;
		if (!PyArg_ParseTuple(args, "i", &i))return NULL;
		if (i < 0 || i>2) {
			PyErr_SetString(PyExc_TypeError, "directional lamp index is 0,1,2");
			return NULL;
		}
		vec_obj* v3robj = PyObject_New(vec_obj, _Vec3Type);
		if (!v3robj) return NULL;
		vmath_cpy(self->envSet->GetPointLampColor(i).P(), 3, v3robj->v);
		v3robj->d = 3;
		return (PyObject*)v3robj;
	}

	static PyObject* environment_setPointLampColor(environment_obj* self, PyObject* args) {
		int i;
		PyObject* value;
		if (!PyArg_ParseTuple(args, "iO", &i, &value))return NULL;
		int d1;
		float buff[4];
		float* v1 = pyObjToFloat((PyObject*)value, buff, d1);
		if (!v1) return NULL;
		self->envSet->SetPointLampColor(i, *((Vec3*)v1));
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* environment_getPointLampPosition(environment_obj* self, PyObject* args) {
		int i;
		if (!PyArg_ParseTuple(args, "i", &i))return NULL;
		if (i < 0 || i>2) {
			PyErr_SetString(PyExc_TypeError, "directional lamp index is 0,1,2");
			return NULL;
		}
		vec_obj* v3robj = PyObject_New(vec_obj, _Vec3Type);
		if (!v3robj) return NULL;
		vmath_cpy(self->envSet->GetPointLampPosition(i).P(), 3, v3robj->v);
		v3robj->d = 3;
		return (PyObject*)v3robj;
	}

	static PyObject* environment_setPointLampPosition(environment_obj* self, PyObject* args) {
		int i;
		PyObject* value;
		if (!PyArg_ParseTuple(args, "iO", &i, &value))return NULL;
		int d1;
		float buff[4];
		float* v1 = pyObjToFloat((PyObject*)value, buff, d1);
		if (!v1) return NULL;
		self->envSet->SetPointLampPosition(i, *((Vec3*)v1));
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject* environment_getdistanceFogNear(environment_obj* self) {
		return PyFloat_FromDouble(self->envSet->GetDistanceFogNear());
	}
	int environment_setdistanceFogNear(environment_obj* self, PyObject* value) {
		if (!(PyFloat_Check(value) || PyLong_Check(value))) {
			PyErr_SetString(PyExc_TypeError, "Only float value can be set to distanceFogNear.");
			return -1;
		}
		self->envSet->SetDistanceFogNear((float)PyFloat_AsDouble(value));
		return 0;
	}

	PyObject* environment_getdistanceFogFar(environment_obj* self) {
		return PyFloat_FromDouble(self->envSet->GetDistanceFogFar());
	}
	int environment_setdistanceFogFar(environment_obj* self, PyObject* value) {
		if (!(PyFloat_Check(value) || PyLong_Check(value))) {
			PyErr_SetString(PyExc_TypeError, "Only float value can be set to distanceFogNear.");
			return -1;
		}
		self->envSet->SetDistanceFogFar((float)PyFloat_AsDouble(value));
		return 0;
	}

	PyObject* environment_getdistanceFogAlpha(environment_obj* self) {
		return PyFloat_FromDouble(self->envSet->GetDistanceFogAlpha());
	}
	int environment_setdistanceFogAlpha(environment_obj* self, PyObject* value) {
		if (!(PyFloat_Check(value) || PyLong_Check(value))) {
			PyErr_SetString(PyExc_TypeError, "Only float value can be set to distanceFogNear.");
			return -1;
		}
		self->envSet->SetDistanceFogAlpha((float)PyFloat_AsDouble(value));
		return 0;
	}

	PyObject* environment_getdistanceFogColor(environment_obj* self)
	{
		vec_obj* v3robj = PyObject_New(vec_obj, _Vec3Type);
		if (!v3robj) return NULL;
		vmath_cpy(self->envSet->GetDistanceFogColor().P(), 3, v3robj->v);
		v3robj->d = 3;
		return (PyObject*)v3robj;
	}

	int environment_setdistanceFogColor(environment_obj* self, PyObject* value)
	{
		int d1;
		float buff[4];
		float* v1 = pyObjToFloat((PyObject*)value, buff, d1);
		if (!v1) return NULL;
		self->envSet->SetDistanceFogColor(*((Vec3*)v1));
		return 0;
	}

	PyMethodDef environment_methods[] = {
		{ "getDirectionalLampIntensity", (PyCFunction)environment_getDirectionalLampIntensity, METH_VARARGS, getDirectionalLampIntensity_doc},
		{ "setDirectionalLampIntensity", (PyCFunction)environment_setDirectionalLampIntensity, METH_VARARGS, setDirectionalLampIntensity_doc},
		{ "getDirectionalLampColor", (PyCFunction)environment_getDirectionalLampColor, METH_VARARGS, getDirectionalLampColor_doc},
		{ "setDirectionalLampColor", (PyCFunction)environment_setDirectionalLampColor, METH_VARARGS, setDirectionalLampColor_doc},
		{ "getDirectionalLampDirection", (PyCFunction)environment_getDirectionalLampDirection, METH_VARARGS, getDirectionalLampDirection_doc},
		{ "setDirectionalLampDirection", (PyCFunction)environment_setDirectionalLampDirection, METH_VARARGS, setDirectionalLampDirection_doc},
		{ "getPointLampRange", (PyCFunction)environment_getPointLampRange, METH_VARARGS, getPointLampRange_doc},
		{ "setPointLampRange", (PyCFunction)environment_setPointLampRange, METH_VARARGS, setPointLampRange_doc},
		{ "getPointLampIntensity", (PyCFunction)environment_getPointLampIntensity, METH_VARARGS, getPointLampIntensity_doc},
		{ "setPointLampIntensity", (PyCFunction)environment_setPointLampIntensity, METH_VARARGS, setPointLampIntensity_doc},
		{ "getPointLampColor", (PyCFunction)environment_getPointLampColor, METH_VARARGS, getPointLampColor_doc},
		{ "setPointLampColor", (PyCFunction)environment_setPointLampColor, METH_VARARGS, setPointLampColor_doc},
		{ "getPointLampPosition", (PyCFunction)environment_getPointLampPosition, METH_VARARGS, getPointLampPosition_doc},
		{ "setPointLampPosition", (PyCFunction)environment_setPointLampPosition, METH_VARARGS, setPointLampPosition_doc},
		{ NULL,	NULL }
	};

	PyGetSetDef environment_getsets[] = {
		{ const_cast<char*>("ambientColor"), (getter)environment_getambientColor, (setter)environment_setambientColor,ambientColor_doc, NULL },
		{ const_cast<char*>("groundColor"), (getter)environment_getgroundColor, (setter)environment_setgroundColor,groundColor_doc, NULL },
		{ const_cast<char*>("ambientDirection"), (getter)environment_getambientDirection, (setter)environment_setambientDirection,ambientDirection_doc, NULL },
		{ const_cast<char*>("distanceFogNear"), (getter)environment_getdistanceFogNear, (setter)environment_setdistanceFogNear,distanceFogNear_doc, NULL },
		{ const_cast<char*>("distanceFogFar"), (getter)environment_getdistanceFogFar, (setter)environment_setdistanceFogFar,distanceFogFar_doc, NULL },
		{ const_cast<char*>("distanceFogAlpha"), (getter)environment_getdistanceFogAlpha, (setter)environment_setdistanceFogAlpha,distanceFogAlpha_doc, NULL },
		{ const_cast<char*>("distanceFogColor"), (getter)environment_getdistanceFogColor, (setter)environment_setdistanceFogColor,distanceFogColor_doc, NULL },
		{ NULL, NULL }
	};

	PyTypeObject EnvironmentType = {
		PyVarObject_HEAD_INIT(NULL, 0)
		"pyxie.environment",				/* tp_name */
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
		environment_doc,					/* tp_doc */
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