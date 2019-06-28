#include "pythonResource.h"
#include "pyVectorMath.h"

namespace pyxie {
	PyTypeObject* _Vec2Type = nullptr;
	PyTypeObject* _Vec3Type = nullptr;
	PyTypeObject* _Vec4Type = nullptr;
	PyTypeObject* _QuatType = nullptr;
	PyTypeObject* _Mat22Type = nullptr;
	PyTypeObject* _Mat33Type = nullptr;
	PyTypeObject* _Mat44Type = nullptr;

	bool ImportVMath() {
		PyObject* mod = PyImport_ImportModule("pyvmath");
		if (!mod) return false;
		_Vec2Type = (PyTypeObject*)PyObject_GetAttrString(mod, "vec2");
		_Vec3Type = (PyTypeObject*)PyObject_GetAttrString(mod, "vec3");
		_Vec4Type = (PyTypeObject*)PyObject_GetAttrString(mod, "vec4");
		_QuatType = (PyTypeObject*)PyObject_GetAttrString(mod, "quat");
		_Mat22Type = (PyTypeObject*)PyObject_GetAttrString(mod, "mat22");
		_Mat33Type = (PyTypeObject*)PyObject_GetAttrString(mod, "mat33");
		_Mat44Type = (PyTypeObject*)PyObject_GetAttrString(mod, "mat44");
		Py_DECREF(mod);
		return true;
	}

	float* pyObjToFloat(PyObject* obj, float* f, int& d) {
		if (f) {
			f[0] = f[1] = f[2] = f[3] = 0.0f;
		}

		if (obj->ob_type == _Vec2Type || obj->ob_type == _Vec3Type || obj->ob_type == _Vec4Type || obj->ob_type == _QuatType) {
			d = ((vec_obj*)obj)->d;
			return ((vec_obj*)obj)->v;
		}
		else if (PyFloat_Check(obj) || PyLong_Check(obj)) {
			f[0] = (float)PyFloat_AsDouble(obj);
			d = 1;
		}
		else if (PyTuple_Check(obj)) {
			d = (int)PyTuple_Size(obj);
			if (d > 4) d = 4;
			for (int j = 0; j < d; j++) {
				PyObject* val = PyTuple_GET_ITEM(obj, j);
				f[j] = (float)PyFloat_AsDouble(val);
			}
		}
		else if (PyList_Check(obj)) {
			d = (int)PyList_Size(obj);
			if (d > 4) d = 4;
			for (int j = 0; j < d; j++) {
				PyObject* val = PyList_GET_ITEM(obj, j);
				f[j] = (float)PyFloat_AsDouble(val);
			}
		}
		else {
			PyErr_SetString(PyExc_ValueError, "invalid arguments");
			return  NULL;
		}
		return f;
	}
}