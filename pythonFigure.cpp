#include "pyxie.h"
#include "pythonResource.h"
#include "pyxieResourceCreator.h"
#include "pyxieTime.h"
#include "pyVectorMath.h"
#include "pythonFigure_doc_en.h"

namespace pyxie
{
	int GetJointIndex(figure_obj* self, PyObject* arg) {
		int index = -1;
		if (PyLong_Check(arg)) {
			index = PyLong_AsLong(arg);
		}
		else if (PyUnicode_Check(arg)) {
			Py_ssize_t data_len;
			const char* key_str = PyUnicode_AsUTF8AndSize(arg, &data_len);
			index = self->figure->GetJointIndex(GenerateNameHash(key_str));
		}
		if (index == -1) {
			PyErr_SetString(PyExc_TypeError, "joint not found.");
		}
		return index;
	}



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
		if (!PyArg_ParseTuple(args, "i", &slot))return NULL;
		float value  = self->figure->GetBlendingWeight(slot);
		return PyFloat_FromDouble(value);
	}


	static PyObject* figure_getJoint(figure_obj* self, PyObject* args) {

		PyObject* arg;
		if (!PyArg_ParseTuple(args, "O", &arg))return NULL;

		int index = GetJointIndex(self, arg);
		if (index == -1) return NULL;
		auto joint = self->figure->GetJoint(index);

		PyObject* joint_obj = PyTuple_New(3);

		vec_obj* pos = (vec_obj*)PyObject_New(vec_obj, _Vec3Type);
		vec_obj* rot = (vec_obj*)PyObject_New(vec_obj, _QuatType);
		vec_obj* scale = (vec_obj*)PyObject_New(vec_obj, _Vec3Type);
		for (int i = 0; i < 4; i++) {
			pos->v[i] = joint.translation[i];


			rot->v[i] = joint.rotation[i];
			scale->v[i] = joint.scale[i];
		}
		PyTuple_SetItem(joint_obj, 0, (PyObject*)pos);
		PyTuple_SetItem(joint_obj, 1, (PyObject*)rot);
		PyTuple_SetItem(joint_obj, 2, (PyObject*)scale);
		return joint_obj;
	}

	static PyObject* figure_setJoint(figure_obj* self, PyObject* args, PyObject* kwargs) {

		static char* kwlist[] = { "jointName","position","rotation","scale", NULL };

		PyObject* arg;
		PyObject* arg1 = nullptr;
		PyObject* arg2 = nullptr;
		PyObject* arg3 = nullptr;
		if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O|OOO", kwlist, &arg, &arg1, &arg2, &arg3))return NULL;

		int index = GetJointIndex(self, arg);
		if (index == -1) return NULL;

		Joint joint = self->figure->GetJoint(index);
		float* v;
		int d;
		float buff[4];
		if (arg1) {
			v = pyObjToFloat(arg1, buff, d);
			for (int i = 0; i < d; i++) joint.translation[i] = v[i];
		}
		if (arg2) {
			v = pyObjToFloat(arg2, buff, d);
			for (int i = 0; i < d; i++) joint.rotation[i] = v[i];
		}
		if (arg3) {
			v = pyObjToFloat(arg3, buff, d);
			for (int i = 0; i < d; i++) joint.scale[i] = v[i];
		}
		self->figure->SetJoint(index, joint);

		Py_INCREF(Py_None);
		return Py_None;
	}


	static PyObject* figure_SetMaterialParam(figure_obj* self, PyObject* args)
	{
		//efig.setMaterialParam("mate01", "DiffuseColor", (1.0, 1.0, 1.0, 1.0));

		char* materialName;
		char* paramName;
		PyObject* param;
		if (!PyArg_ParseTuple(args, "ssO", &materialName, &paramName, &param)) return NULL;

		float buff[4];

		Py_ssize_t size = 0;
		ShaderParameterDataType type = ParamTypeUnknown;
		if (PyTuple_Check(param)) {
			size = PyTuple_Size(param);
			if (size <= 0 || size > 4) {
				PyErr_SetString(PyExc_TypeError, "parameter error.");
				return NULL;
			}
			for (Py_ssize_t i = 0; i < size; i++) {
				auto item = PyTuple_GET_ITEM(param, i);
				if (!(PyFloat_Check(item)| PyLong_Check(item))) {
					PyErr_SetString(PyExc_TypeError, "parameter error.");
					return NULL;
				}
				buff[i] = (float)PyFloat_AsDouble(item);
			}
		}
		else if (PyList_Check(param)) {
			size = PyList_Size(param);
			if (size <= 0 || size > 4) {
				PyErr_SetString(PyExc_TypeError, "parameter error.");
				return NULL;
			}
			for (Py_ssize_t i = 0; i < size; i++) {
				auto item = PyList_GET_ITEM(param, i);
				if (!(PyFloat_Check(item) | PyLong_Check(item))) {
					PyErr_SetString(PyExc_TypeError, "parameter error.");
					return NULL;
				}
				buff[i] = (float)PyFloat_AsDouble(item);
			}
		}
		else if (PyFloat_Check(param)) {
			size = 1;
			buff[0] = (float)PyFloat_AsDouble(param);
		}
		if (size == 0) {
			PyErr_SetString(PyExc_TypeError, "parameter error.");
			return NULL;
		}

		switch (size) {
		case 1: type = ParamTypeFloat; break;
		case 2: type = ParamTypeFloat2; break;
		case 3: type = ParamTypeFloat3; break;
		case 4: type = ParamTypeFloat4; break;
		}
		if (!self->figure->SetMaterialParam(materialName, paramName, buff)) {
			PyErr_SetString(PyExc_TypeError, "parameter error.");
			return NULL;
		}
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* figure_SetMaterialParamTexture(figure_obj* self, PyObject* args, PyObject* kwargs)
	{
		static char* kwlist[] = { "materialName","samplerName","textureName","pixel","width","height","wrap_s","wrap_t","minfilter","magfilter","mipfilter", NULL };

		char* materialName = nullptr;
		char* samplerName = nullptr;

		PyObject* textureName = nullptr;

		PyObject* pixel = nullptr;
		int w = 0;
		int h = 0;
		int wrap_s = SamplerState::WRAP;
		int wrap_t = SamplerState::WRAP;
		int minfilter = SamplerState::LINEAR_MIPMAP_LINEAR;
		int magfilter = SamplerState::LINEAR;
		int mipfilter = SamplerState::LINEAR_MIPMAP_LINEAR;

		if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ssO|Oiiiiiii", kwlist,
			&materialName, &samplerName, &textureName,
			&pixel, &w, &h,
			&wrap_s, &wrap_t, &minfilter, &magfilter, &mipfilter)) return NULL;

		Sampler sampler;
		if (textureName->ob_type == &TextureType) {
			((texture_obj*)textureName)->colortexture->IncReference();
			sampler.tex = ((texture_obj*)textureName)->colortexture;
		}
		else {
			const char* nameText = PyUnicode_AsUTF8(textureName);
			if (pixel) {
				if (!PyBytes_Check(pixel) || w == 0 || h == 0) {
					PyErr_SetString(PyExc_TypeError, "parameter error");
					return NULL;
				}
				char* pix = PyBytes_AsString(pixel);
				sampler.tex = pyxieResourceCreator::Instance().NewTexture(nameText, pix, w, h, true);
			}
			else {
				sampler.tex = pyxieResourceCreator::Instance().NewTexture(nameText);
			}
		}
		sampler.samplerSlot = 0;
		sampler.samplerState.wrap_s = wrap_s;
		sampler.samplerState.wrap_t = wrap_t;
		sampler.samplerState.minfilter = minfilter;
		sampler.samplerState.magfilter = magfilter;
		sampler.samplerState.mipfilter = mipfilter;

		if (!self->figure->SetMaterialParam(materialName, samplerName, &sampler)) {
			if (sampler.tex)sampler.tex->DecReference();
			PyErr_SetString(PyExc_TypeError, "parameter error.");
			return NULL;
		}
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* figure_GetMaterialParam(figure_obj* self, PyObject* args) {

		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* figure_SetParentJoint(figure_obj* self, PyObject* args) {

		PyObject* parent;
		char* jointName;
		if (!PyArg_ParseTuple(args, "Os", &parent, &jointName)) return NULL;

		auto drawable = ((pyxieDrawable*)(((resource_obj*)parent)->res));
		uint32_t hash = GenerateNameHash(jointName);
		int idx = drawable->GetJointIndex(hash);
		if (idx != -1) {
			self->figure->SetParentJoint(drawable, drawable->GetJointMatrix(idx));
		}
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyMethodDef figure_methods[] = {
		{ "connectAnimator", (PyCFunction)figure_BindAnimator, METH_VARARGS, connectAnimator_doc},
		{ "getCamera", (PyCFunction)figure_GetCamera, METH_VARARGS, getCamera_doc},
		{ "getEnvironment", (PyCFunction)figure_GetEnvironment, METH_NOARGS, getEnvironment_doc},
		{ "step", (PyCFunction)figure_Step, METH_VARARGS, step_doc},
		{ "setTime", (PyCFunction)figure_SetTime, METH_VARARGS, setTime_doc},
		{ "setBlendingWeight", (PyCFunction)figure_setBlendingWeight, METH_VARARGS, setBlendingWeight_doc},
		{ "getBlendingWeight", (PyCFunction)figure_getBlendingWeight, METH_VARARGS, getBlendingWeight_doc},
		{ "getJoint", (PyCFunction)figure_getJoint, METH_VARARGS, getJoint_doc},
		{ "setJoint", (PyCFunction)figure_setJoint, METH_VARARGS | METH_KEYWORDS, setJoint_doc},
		{ "getMaterialParam", (PyCFunction)figure_GetMaterialParam, METH_VARARGS, getMaterialParam_doc},
		{ "setMaterialParam", (PyCFunction)figure_SetMaterialParam, METH_VARARGS, setMaterialParam_doc},
		{ "setMaterialParamTexture", (PyCFunction)figure_SetMaterialParamTexture, METH_VARARGS | METH_KEYWORDS, setMaterialParamTexture_doc},
		{ "setParentJoint", (PyCFunction)figure_SetParentJoint, METH_VARARGS, setParentJoint_doc},

		//{ "dump", (PyCFunction)figure_Dump, METH_VARARGS },

	{ NULL,	NULL }
	};

	PyGetSetDef figure_getsets[] = {
		{ const_cast<char*>("position"), (getter)figure_getPosition, (setter)figure_setPosition,position_doc, NULL },
		{ const_cast<char*>("rotation"), (getter)figure_getRotation, (setter)figure_setRotation,rotation_doc, NULL },
		{ const_cast<char*>("scale"),    (getter)figure_getScale,    (setter)figure_setScale,scale_doc, NULL },
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