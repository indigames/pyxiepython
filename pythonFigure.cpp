#include "pyxie.h"
#include "pythonResource.h"
#include "pyxieResourceCreator.h"
#include "pyxieTime.h"
#include "pyVectorMath.h"

namespace pyxie
{
	PyObject *figure_new(PyTypeObject *type, PyObject *args, PyObject *kw) {
		char* path;
		figure_obj * self = NULL;

		if (PyArg_ParseTuple(args, "s", &path)) {
			self = (figure_obj*)type->tp_alloc(type, 0);
			self->figure = pyxieResourceCreator::Instance().NewFigure(path);
		}
		return (PyObject *)self;
	}

	void  figure_dealloc(figure_obj *self)
	{
		self->figure->DecReference();
		Py_TYPE(self)->tp_free(self);
	}

	PyObject *figure_str(figure_obj *self)
	{
		char buf[64];
		pyxie_snprintf(buf, 64, "figure object");
		return _PyUnicode_FromASCII(buf, strlen(buf));
	}

	PyObject* figure_getPosition(figure_obj* self)
	{
		vec_obj* v3robj = PyObject_New(vec_obj, _Vec3Type);
		if (!v3robj) return NULL;
		vmath_cpy(self->figure->GetPosition().P(), 3, v3robj->v);
		v3robj->d = 3;
		return (PyObject*)v3robj;
	}
	int figure_setPosition(figure_obj* self, PyObject* value)
	{
		int d1;
		float buff[4];
		float* v1 = pyObjToFloat((PyObject*)value, buff, d1);
		if (!v1) return NULL;
		self->figure->SetPosition(*((Vec3*)v1));
		return 0;
	}

	PyObject* figure_getRotation(figure_obj* self)
	{
		vec_obj* quatobj = PyObject_New(vec_obj, _QuatType);
		if (!quatobj) return NULL;
		vmath_cpy(self->figure->GetRotation().P(), 4, quatobj->v);
		quatobj->d = 4;
		return (PyObject*)quatobj;
	}
	int figure_setRotation(figure_obj* self, PyObject* value)
	{
		int d1;
		float buff[4];
		float* v1 = pyObjToFloat((PyObject*)value, buff, d1);
		if (!v1) return NULL;
		self->figure->SetRotation(*((Quat*)v1));
		return 0;
	}

	PyObject* figure_getScale(figure_obj* self)
	{
		vec_obj* v3robj = PyObject_New(vec_obj, _Vec3Type);
		if (!v3robj) return NULL;
		vmath_cpy(self->figure->GetScale().P(), 3, v3robj->v);
		v3robj->d = 3;
		return (PyObject*)v3robj;
	}
	int figure_setScale(figure_obj* self, PyObject* value)
	{
		int d1;
		float buff[4];
		float* v1 = pyObjToFloat((PyObject*)value, buff, d1);
		if (!v1) return NULL;
		self->figure->SetScale(*((Vec3*)v1));
		return 0;
	}

	static PyObject *figure_BindAnimator(figure_obj *self, PyObject *args)
	{
		int slot = 0;
		PyObject* arg2=nullptr;

		if (PyArg_ParseTuple(args, "i|O", &slot, &arg2)){
			if (arg2) {
				if (PyUnicode_Check(arg2)) {
					const char* motionName = PyUnicode_AsUTF8(arg2);
					self->figure->BindAnimator((pyxieFigure::AnimatorSlot)slot, motionName);
				}
				else if (arg2->ob_type == &AnimatorType) {
					animator_obj* anime = (animator_obj*)arg2;
					self->figure->BindAnimator((pyxieFigure::AnimatorSlot)slot, anime->anime);
				}
				else{
					PyErr_SetString(PyExc_TypeError, "Argument of connectAnimator must be (integer, animator) or (integer) if unbind.");
					return NULL;
				}
			}
			else {
				self->figure->BindAnimator((pyxieFigure::AnimatorSlot)slot, (pyxieAnimator*)nullptr);
			}
		}
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject *figure_GetCamera(figure_obj *self, PyObject *args)
	{
		char* name = NULL;
		if (!PyArg_ParseTuple(args, "|s", &name))
			return NULL;

		camera_obj *obj = PyObject_New(camera_obj, &CameraType);
		obj->camera = pyxieResourceCreator::Instance().NewCamera(name, self->figure);
		return (PyObject *)obj;
	}

	static PyObject *figure_GetEnvironment(figure_obj *self)
	{
		environment_obj *obj = PyObject_New(environment_obj, &EnvironmentType);
		obj->envSet = pyxieResourceCreator::Instance().NewEnvironmentSet(self->figure);
		return (PyObject *)obj;
	}

	static PyObject *figure_Step(figure_obj *self, PyObject *args){
		float s = FLT_MAX;
		if (!PyArg_ParseTuple(args, "|f", &s))return NULL;
		if (s == FLT_MAX) s = (float)pyxieTime::Instance().GetElapsedTime();
		self->figure->Step(s);
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* figure_SetTime(figure_obj* self, PyObject* args) {
		float s;
		if (!PyArg_ParseTuple(args, "f", &s))return NULL;
		self->figure->SetEvalTime(s);
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* figure_Dump(figure_obj* self, PyObject* args) {
		char* file;
		if (!PyArg_ParseTuple(args, "s", &file))return NULL;
		self->figure->Dump(file);
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* figure_setBlendingWeight(figure_obj* self, PyObject* args) {
		int slot;
		float value;
		if (!PyArg_ParseTuple(args, "if", &slot, &value))return NULL;
		self->figure->SetBlendingWeight(slot, value);
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* figure_getBlendingWeight(figure_obj* self, PyObject* args) {
		int slot;
		if (!PyArg_ParseTuple(args, "slot", &slot))return NULL;
		float value  = self->figure->GetBlendingWeight(slot);
		return PyFloat_FromDouble(value);
	}

	PyMethodDef figure_methods[] = {
		{ "connectAnimator", (PyCFunction)figure_BindAnimator, METH_VARARGS },
		{ "getCamera", (PyCFunction)figure_GetCamera, METH_VARARGS },
		{ "getEnvironment", (PyCFunction)figure_GetEnvironment, METH_NOARGS },
		{ "step", (PyCFunction)figure_Step, METH_VARARGS },
		{ "setTime", (PyCFunction)figure_SetTime, METH_VARARGS },
		{ "setBlendingWeight", (PyCFunction)figure_setBlendingWeight, METH_VARARGS },
		{ "getBlendingWeight", (PyCFunction)figure_getBlendingWeight, METH_VARARGS },
		//{ "dump", (PyCFunction)figure_Dump, METH_VARARGS },

	{ NULL,	NULL }
	};

	PyGetSetDef figure_getsets[] = {
		{ const_cast<char*>("position"), (getter)figure_getPosition, (setter)figure_setPosition,NULL, NULL },
		{ const_cast<char*>("rotation"), (getter)figure_getRotation, (setter)figure_setRotation,NULL, NULL },
		{ const_cast<char*>("scale"),    (getter)figure_getScale,    (setter)figure_setScale,NULL, NULL },
		{ NULL, NULL }
	};

	PyTypeObject FigureType = {
		PyVarObject_HEAD_INIT(NULL, 0)
		"pyxie.figure",						/* tp_name */
		sizeof(figure_obj),                 /* tp_basicsize */
		0,                                  /* tp_itemsize */
		(destructor)figure_dealloc,			/* tp_dealloc */
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
		(reprfunc)figure_str,               /* tp_str */
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
		figure_methods,						/* tp_methods */
		0,                                  /* tp_members */
		figure_getsets,                     /* tp_getset */
		0,                                  /* tp_base */
		0,                                  /* tp_dict */
		0,                                  /* tp_descr_get */
		0,                                  /* tp_descr_set */
		0,                                  /* tp_dictoffset */
		0,                                  /* tp_init */
		0,                                  /* tp_alloc */
		figure_new,							/* tp_new */
		0,									/* tp_free */
	};

}