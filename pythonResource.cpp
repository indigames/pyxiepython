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


	int pyObjToFloatArray(PyObject* obj, float* f, int numElement) {

		if (numElement > 4) numElement = 4;

		int totalCount = 0;
		int type = -1;
		if (PyTuple_Check(obj)) type = 0;
		else if (PyList_Check(obj))  type = 1;
		if (type == -1) return 0;

		int elementCount = 0;
		int numElem = (type == 0) ? PyTuple_Size(obj) : PyList_Size(obj);
		for (int i = 0; i < numElem; i++) {
			PyObject* element = (type == 0) ? PyTuple_GET_ITEM(obj, i) : PyList_GET_ITEM(obj, i);
			if (element->ob_type == _Vec2Type || element->ob_type == _Vec3Type || element->ob_type == _Vec4Type || element->ob_type == _QuatType) {
				int d = ((vec_obj*)element)->d;
				float* v = ((vec_obj*)element)->v;
				for (int j = 0; j < d; j++) {
					if(f)f[totalCount] = v[j];
					totalCount++;
					elementCount++;
					if (elementCount >= numElement) break;
				}
				for (int j = elementCount; j < numElement; j++) {
					if (f)f[totalCount] = 0.0f;
					totalCount++;
				}
				elementCount=0;
			}
			else if (PyFloat_Check(element) || PyLong_Check(element)) {
				if (f)f[totalCount] = (float)PyFloat_AsDouble(element);
				totalCount++;
				elementCount++;
				if (elementCount >= numElement) elementCount=0;
			}
			else if (PyTuple_Check(element)) {
				int d = (int)PyTuple_Size(element);
				for (int j = 0; j < d; j++) {
					PyObject* val = PyTuple_GET_ITEM(element, j);
					if (f)f[totalCount] = (float)PyFloat_AsDouble(val);
					totalCount++;
					elementCount++;
					if (elementCount >= numElement) break;
				}
				for (int j = elementCount; j < numElement; j++) {
					if (f)f[totalCount] = 0.0f;
					totalCount++;
				}
				elementCount = 0;
			}
			else if (PyList_Check(element)) {
				int d = (int)PyList_Size(element);
				if (d > 4) d = 4;
				for (int j = 0; j < d; j++) {
					PyObject* val = PyList_GET_ITEM(element, j);
					if (f)f[totalCount] = (float)PyFloat_AsDouble(val);
					totalCount++;
					elementCount++;
					if (elementCount >= numElement) break;
				}
				for (int j = elementCount; j < numElement; j++) {
					if (f)f[totalCount] = 0.0f;
					totalCount++;
				}
				elementCount = 0;
			}
		}
		if (elementCount > 0 && elementCount < elementCount) {
			for (int j = elementCount; j < numElement; j++) {
				if (f)f[totalCount] = 0.0f;
				totalCount++;
			}
		}
		return totalCount;
	}



}