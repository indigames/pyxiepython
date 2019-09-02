#include "pyxie.h"
#include "pythonResource.h"
#include "pyxieResourceCreator.h"
#include "Backyard.h"
#include "pyVectorMath.h"
#include "pythonTexture_doc_en.h"
#include "ndarrayobject.h"

namespace pyxie
{
	PyObject *texture_new(PyTypeObject *type, PyObject *args, PyObject *kw) {

		static char* kwlist[] = { "name","width","height","alpha","depth","stencil","pixel", NULL };

		char* name;
		int width = 0;
		int height = 0;
		int alpha = 0;
		int depth = 0;
		int stencil =0;
		PyObject* pixel = nullptr;

		if (!PyArg_ParseTupleAndKeywords(args, kw, "s|iipppO", kwlist,
			&name, &width, &height, &alpha, &depth, &stencil, &pixel)) return NULL;

		texture_obj* self = NULL;

		char* pix = nullptr;
		if (pixel) {
			if (pixel->ob_type->tp_name && strcmp(pixel->ob_type->tp_name, "numpy.ndarray") == 0) {
				PyArrayObject_fields* ndarray = (PyArrayObject_fields*)pixel;
				height = *ndarray->dimensions;
				width = *ndarray->strides / ndarray->nd;
				alpha = ndarray->nd == 4 ? true : false;
				pix = ndarray->data;
			}
			else if (PyBytes_Check(pixel)) {
				pix = PyBytes_AsString(pixel);
			}
		}

		self = (texture_obj*)type->tp_alloc(type, 0);

		if (width == 0 || height == 0)
			self->colortexture = pyxieResourceCreator::Instance().NewTexture(name);
		else
			self->colortexture = pyxieResourceCreator::Instance().NewTexture(name, pix, width, height, alpha);

		self->depth = depth;
		self->stencil = stencil;
		self->renderTarget = nullptr;

		return (PyObject *)self;
	}

	void  texture_dealloc(texture_obj*self)
	{
		self->colortexture->DecReference();
		if(self->renderTarget) self->renderTarget->DecReference();
		Py_TYPE(self)->tp_free(self);
	}

	PyObject * texture_str(texture_obj *self)
	{
		char buf[64];
		pyxie_snprintf(buf, 64, "texture object");
		return _PyUnicode_FromASCII(buf, strlen(buf));
	}

	static PyObject* texture_setImage(texture_obj* self, PyObject* args, PyObject* kwargs)
	{
		static char* kwlist[] = { "image","x","y","width","height", NULL };

		self->subImage = NULL;
		self->x = 0;
		self->y = 0;
		self->w = -1;
		self->h = -1;
		if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O|iiii", kwlist,
			&(self->subImage), &(self->x), &(self->y), &(self->w), &(self->h))) return NULL;

		Backyard::Instance().UpdateImageRequest(self);

		if (self->subImage) {
			Py_INCREF(self->subImage);
		}

		Py_INCREF(Py_None);
		return Py_None;
	}

	PyMethodDef texture_methods[] = {
		{ "setImage", (PyCFunction)texture_setImage, METH_VARARGS | METH_KEYWORDS,setImage_doc },
		{ NULL,	NULL }
	};

	PyGetSetDef texture_getsets[] = {
		{ NULL, NULL }
	};

	PyTypeObject TextureType = {
		PyVarObject_HEAD_INIT(NULL, 0)
		"pyxie.texture",					/* tp_name */
		sizeof(texture_obj),                /* tp_basicsize */
		0,                                  /* tp_itemsize */
		(destructor)texture_dealloc,		/* tp_dealloc */
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
		(reprfunc)texture_str,               /* tp_str */
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
		texture_methods,						/* tp_methods */
		0,                                  /* tp_members */
		texture_getsets,                     /* tp_getset */
		0,                                  /* tp_base */
		0,                                  /* tp_dict */
		0,                                  /* tp_descr_get */
		0,                                  /* tp_descr_set */
		0,                                  /* tp_dictoffset */
		0,                                  /* tp_init */
		0,                                  /* tp_alloc */
		texture_new,							/* tp_new */
		0,									/* tp_free */
	};

}