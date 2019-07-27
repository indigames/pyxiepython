#include "pyxie.h"
#include "pythonResource.h"

namespace pyxie
{
	typedef struct {
		PyObject_HEAD
	} pyxietools_obj;

	PyTypeObject* _EditableFigureType = nullptr;

	static PyObject* tools_loadCollada(pyxietools_obj* self, PyObject* args, PyObject* kwargs) {
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* tools_loadColladaAnimation(pyxietools_obj* self, PyObject* args, PyObject* kwargs) {
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* tools_convertTextureToPlatform(pyxietools_obj* self, PyObject* args) {
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* tools_compressFolder(pyxietools_obj* self, PyObject* args) {
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyMethodDef tools_methods[] = {
		{ "loadCollada", (PyCFunction)tools_loadCollada, METH_VARARGS | METH_KEYWORDS },
		{ "loadColladaAnimation", (PyCFunction)tools_loadColladaAnimation, METH_VARARGS | METH_KEYWORDS },
		{ "convertTextureToPlatform", (PyCFunction)tools_convertTextureToPlatform, METH_VARARGS },
		{ "compressFolder", (PyCFunction)tools_compressFolder, METH_VARARGS },
		{ NULL,	NULL }
	};

	static PyModuleDef tools_module = {
		PyModuleDef_HEAD_INIT,
		"pyxietools",								// Module name to use with Python import statements
		"pyxie deverop tool",		// Module description
		0,
		tools_methods								// Structure that defines the methods of the module
	};

	PyMODINIT_FUNC PyInit__pyxietools() {
		PyObject* module = PyModule_Create(&tools_module);
		return module;
	}


}




