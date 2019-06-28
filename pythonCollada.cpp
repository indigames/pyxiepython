#include "pyxie.h"
#include "pyxieObject.h"
#include "pythonTools.h"
#include "pythonResource.h"
#include "pyxieResourceCreator.h"

namespace pyxie
{
	class Loader : public pyxieObject{
	public:
		static ColladaLoader* NewColladaLoader() { return new ColladaLoader; }
		static void DeleteColladaLoader(ColladaLoader* loader) { PYXIE_SAFE_DELETE(loader); }
	};

	PyObject *collada_new(PyTypeObject *type, PyObject *args, PyObject *kw) {
		colladaloader_obj* self = NULL;
		self = (colladaloader_obj*)type->tp_alloc(type, 0);
		self->loader = Loader::NewColladaLoader();
		return (PyObject *)self;
	}

	void  collada_dealloc(colladaloader_obj*self){
		Loader::DeleteColladaLoader(self->loader);
		PyObject_Del(self);
	}

	PyObject * collada_str(colladaloader_obj*self){
		char buf[64];
		pyxie_snprintf(buf, 64, "collada loader object");
		return _PyUnicode_FromASCII(buf, strlen(buf));
	}

	static PyObject* collada_loadCollada(colladaloader_obj* self, PyObject* args){

		const char* path;
		editablefigure_obj* efig;

		if (PyArg_ParseTuple(args, "sO", &path, &efig)) {
			if (Py_TYPE(efig) != &EditableFigureType) {
				PyErr_SetString(PyExc_TypeError, "Argument of loadCollada must be (string, editableFigure).");
				return NULL;
			}
		}
		else return NULL;

		auto rv = self->loader->LoadCollada(path, efig->editablefigure);
		if (!rv) {
			PyErr_SetString(PyExc_TypeError, "Failer to load file.");
			return NULL;
		}

		Py_INCREF(Py_None);
		return Py_None;

	}

	static PyObject* collada_getTexturePathes(figure_obj* self)
	{
		environment_obj* obj = PyObject_New(environment_obj, &EnvironmentType);
		obj->envSet = ResourceCreator::Instance().NewEnvironmentSet(self->figure);
		return (PyObject*)obj;
	}



	PyMethodDef collada_methods[] = {
		{ "loadCollada", (PyCFunction)collada_loadCollada, METH_VARARGS },
		{ "getTexturePathes", (PyCFunction)collada_getTexturePathes, METH_NOARGS },
		{ NULL,	NULL }
	};

	PyTypeObject ColladaLoaderType = {
		PyVarObject_HEAD_INIT(NULL, 0)
		"pyxtools.colladaLoader",			/* tp_name */
		sizeof(colladaloader_obj),          /* tp_basicsize */
		0,                                  /* tp_itemsize */
		(destructor)collada_dealloc,		/* tp_dealloc */
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
		(reprfunc)collada_str,              /* tp_str */
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
		collada_methods,					/* tp_methods */
		0,                                  /* tp_members */
		0,									/* tp_getset */
		0,                                  /* tp_base */
		0,                                  /* tp_dict */
		0,                                  /* tp_descr_get */
		0,                                  /* tp_descr_set */
		0,                                  /* tp_dictoffset */
		0,                                  /* tp_init */
		0,                                  /* tp_alloc */
		collada_new,						/* tp_new */
		0,									/* tp_free */
	};
}