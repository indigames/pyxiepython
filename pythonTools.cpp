#include "pyxie.h"
#include "pythonResource.h"
#include "pyxieResourceCreator.h"
#include "pyxieImageConv.h"
#include "pyxieDatabaseBuilder.h"
#include "pyxieFigureExportConfigManager.h"
#include "pyxieColladaLoader.h"

namespace pyxie
{
	typedef struct {
		PyObject_HEAD
	} pyxietools_obj;

	PyTypeObject* _EditableFigureType = nullptr;

	bool ImportPyxie() {
		PyObject* mod = PyImport_ImportModule("pyxie");
		if (!mod) return false;
		_EditableFigureType = (PyTypeObject*)PyObject_GetAttrString(mod, "editableFigure");
		Py_DECREF(mod);
		return true;
	}


	static PyObject* tools_loadCollada(pyxietools_obj* self, PyObject* args, PyObject* kwargs) {

		static char* kwlist[] = { "filePath", "editableFigure","baseScale", NULL };

		const char* path;
		editablefigure_obj* efig;
		float baseScale = 1.0f;

		if (PyArg_ParseTupleAndKeywords(args, kwargs, "sO|f", kwlist, &path, &efig, &baseScale)) {
			if (Py_TYPE(efig) != _EditableFigureType) {
				PyErr_SetString(PyExc_TypeError, "Argument of loadCollada must be (string, editableFigure (baseScale)).");
				return NULL;
			}
		}
		else return NULL;

		pyxieFigureExportConfigManager::Instance().SetBaseScale(baseScale);
		pyxieColladaLoader loader;
		auto rv = loader.LoadCollada(path, efig->editablefigure);
		if (!rv) {
			PyErr_SetString(PyExc_TypeError, "Failer to load file.");
			return NULL;
		}

		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* tools_loadColladaAnimation(pyxietools_obj* self, PyObject* args, PyObject* kwargs) {

		static char* kwlist[] = { "filePath", "editableFigure","baseScale", NULL };
		const char* path;
		editablefigure_obj* efig;
		float baseScale = 1.0f;

		if (PyArg_ParseTupleAndKeywords(args, kwargs,"sO|f", kwlist,  &path, &efig, &baseScale)) {
			if (Py_TYPE(efig) != _EditableFigureType) {
				PyErr_SetString(PyExc_TypeError, "Argument of loadCollada must be (string, editableFigure (baseScale)).");
				return NULL;
			}
		}
		else return NULL;

		pyxieFigureExportConfigManager::Instance().SetBaseScale(baseScale);
		pyxieColladaLoader loader;
		auto rv = loader.LoadColladaAnimation(path, efig->editablefigure);
		if (!rv) {
			PyErr_SetString(PyExc_TypeError, "Failer to load file.");
			return NULL;
		}

		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* tools_convertTextureToPlatform(pyxietools_obj* self, PyObject* args) {

		char* inFile;
		char* outFile;
		int platform;
		int normal, wrap;

		if (PyArg_ParseTuple(args, "ssiii", &inFile, &outFile, &platform, &normal, &wrap)) {

			char EXT[MAX_PATH];
			char OUT_PATH[MAX_PATH];
			pyxie_strncpy(OUT_PATH, outFile, MAX_PATH);
			GetPartOfFilePath(OUT_PATH, ExtentionToGetFromPath, EXT);
			if (*EXT == 0) pyxie_strncat(OUT_PATH, ".pyxi", MAX_PATH);

			pyxie::ImageConv imgConv;
			imgConv.SetInputFile(inFile);
			imgConv.SetOutputFile(OUT_PATH);
			imgConv.SetIsNormalmap(normal);
			imgConv.SetWrapRepeat(wrap);
			imgConv.SetTargetPlatform(platform);
			imgConv.SetAutoDetectAlpha();
			imgConv.DoConvert();
		}
		Py_INCREF(Py_None);
		return Py_None;

	}

	static PyObject* tools_compressFolder(pyxietools_obj* self, PyObject* args) {
		char* folder;
		if (PyArg_ParseTuple(args, "s", &folder)) {
			pyxie::ContractDatabase(folder, folder);
		}
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
		"Provides some functions, but faster",		// Module description
		0,
		tools_methods								// Structure that defines the methods of the module
	};

	PyMODINIT_FUNC PyInit__pyxietools() {
		PyObject* module = PyModule_Create(&tools_module);

		if (!ImportPyxie()) {
			PyErr_SetString(PyExc_TypeError, "pyvmath isn't installed. please  pip install pyvmath before.");
			return NULL;
		}
		return module;
	}


}




